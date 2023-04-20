using Robocode;
using Robocode.Util;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing.Printing;
using System.Linq;

namespace CAP4053.Student
{
    public class Cooks : TeamRobot
    {

        Mode mode = Mode.Intel;
        enum Mode
        {
            Charge, Dodge, Retreat, Intel
        }

        FSM fsm;
        OtherBot target;
        int shotCounter, hitCounter, ramCount, counterThreshold;
        Random rand;

        //Finite State Machine
        class FSM
        {
            public Cooks robot;
            Dictionary<Mode, State> states;
            State current, start;
            Mode currentMode;
            Event blankEvent;
            int scanLimit;

            Queue<ScannedRobotEvent> scans;
            Queue<HitByBulletEvent> hits;

            //Returns current state (in mode type)
            public Mode State() { return states.FirstOrDefault(mode => mode.Value == current).Key; }

            public void Init()
            {
                states = new Dictionary<Mode, State>() { { Mode.Intel, new Intel() }, { Mode.Charge, new Charge() }, { Mode.Dodge, new Dodge() }, { Mode.Retreat, new Retreat() } };
                start = states[Mode.Intel];
                scanLimit = 1;
                scans = new Queue<ScannedRobotEvent>();
                hits = new Queue<HitByBulletEvent>();
                current = start;
                currentMode = Mode.Intel;
                current.Start(robot);
                //blankEvent = new Event();
                //blankEvent.Priority = 1;
            }
            public void Update()
            {
                ScannedRobotEvent scan = null;
                if (scans.Count > 0)
                {
                    scan = scans.Peek();
                    scans.Dequeue();
                }

                HitByBulletEvent hit = null;
                if (hits.Count > 0)
                {
                    hit = hits.Peek();
                    hits.Dequeue();
                }

                current.Update(ref scan, ref hit);

                robot.Execute();
            }

            public void Shutdown()
            {
                states[Mode.Intel] = null;
                states[Mode.Charge] = null;
                states[Mode.Dodge] = null;
                states[Mode.Retreat] = null;
                states = null;
            }

            public void Transition(Mode before, Mode after)
            {
                states[before].Exit();
                current = states[after];
                states[after].Start(robot);
            }

            public void EnqueueScan(ScannedRobotEvent e)
            {
                if (scans.Count == scanLimit)
                    scans.Dequeue();
                scans.Enqueue(e);
            }

            public void EnqueueHit(HitByBulletEvent e)
            {
                hits.Enqueue(e);
            }
        }

        internal abstract class State
        {
            protected Cooks robot;
            public abstract void Start(Cooks robot);
            public abstract void Update(ref ScannedRobotEvent scan, ref HitByBulletEvent hit);
            public abstract void Exit();
        }

        class Charge : State
        {
            Mode mode;
            public override void Start(Cooks newRobot)
            {
                mode = Mode.Charge;
                robot = newRobot;
                robot.hitCounter = 0;
                robot.shotCounter = 0;
                robot.ramCount = 0;
            }

            public override void Update(ref ScannedRobotEvent scan, ref HitByBulletEvent hit)
            {
                Console.WriteLine("CHARGE");
                double targetEnergy = 0;
                //Scan robot event
                if (scan != null)
                {
                    robot.SetTurnRight(scan.Bearing);
                    robot.SetTurnGunRight(scan.Bearing + (robot.Heading - robot.GunHeading));
                    robot.SetAhead(scan.Distance);
                    targetEnergy = scan.Energy;
                    robot.FireShots(scan.Distance);
                }

                //Hit robot event
                if (hit != null)
                {
                    //if (robot.hitCounter == robot.counterThreshold)
                      //  robot.fsm.Transition(mode, Mode.Retreat);
                }

                //Start dodging after ram (if target is still healthy)
                if (robot.ramCount == 1 && targetEnergy > 40)
                    robot.fsm.Transition(mode, Mode.Dodge);

                robot.FireShots(robot.target.GetDistance());

                if (robot.shotCounter == robot.counterThreshold)
                {
                    robot.fsm.Transition(mode, Mode.Intel);
                }
            }

            public override void Exit()
            {
                
            }
        }

        class Dodge : State
        {
            Mode mode;
            double dodgeDist, dodgeCount, dodgeScale;
            public override void Start(Cooks newRobot)
            {
                mode = Mode.Dodge;
                robot = newRobot;
                robot.hitCounter = 0;
                dodgeScale = 1;
                dodgeDist = robot.rand.Next(100, 200);
                robot.SetAhead(dodgeDist * dodgeScale);
            }

            public override void Update(ref ScannedRobotEvent scan, ref HitByBulletEvent hit)
            {
                Console.WriteLine("Dodging");
                //Dodging and shooting
                int dodgeAngle = 45;
                
                //Scanned Robot Event
                if (scan != null)
                {
                    //Keep within 150-200 pixels
                    if (scan.Distance < 150 || scan.Distance > 200)
                    {
                        robot.SetTurnRight(scan.Bearing);

                        //If I'm close to a wall, move passed the enemy
                        if (robot.X < 150 || robot.X > robot.BattleFieldWidth - 150 || robot.Y < 150 || robot.Y > robot.BattleFieldHeight - 150)
                            robot.SetAhead(150 + scan.Distance);

                        //Otherwise, back up
                        else
                            robot.SetBack(150 - scan.Distance);
                    }
                    else
                    {
                        robot.SetTurnRight(scan.Bearing + 90);

                        //Start another dodge once current is finished
                        if (robot.DistanceRemaining == 0)
                        {
                            dodgeCount++;
                            dodgeScale *= -1;
                            dodgeDist = robot.rand.Next(100, 200);
                            robot.SetAhead(dodgeDist * dodgeScale);
                        }
                    }

                    //Aim and fire at target's current location
                    double gunAngle = scan.Bearing + (robot.Heading - robot.GunHeading);
                    //if (gunAngle > 180)
                        robot.SetTurnGunRight(gunAngle);
                    robot.FireShots(scan.Distance);

                    //Charge again when enemy energy goes low
                    if (scan.Energy < 30 && robot.Energy > 40)
                    //Charge again after 3 dodges
                    //if (dodgeCount == 3)
                        robot.fsm.Transition(mode, Mode.Charge);
                }


                //Hit By Bullet Event
                if (hit != null)
                {
                    if (robot.Energy < 30)
                        robot.fsm.Transition(mode, Mode.Retreat);
                }
                
                /*
                if (Math.Abs(robot.target.GetBearing()) < 90)
                    robot.SetTurnRight(robot.target.GetBearing() + 90);
                */


                robot.Execute();
            }

            public override void Exit()
            {
                //Wait for everything to finish
                //while (robot.DistanceRemaining > 0 || robot.TurnRemaining > 0 || robot.GunTurnRemaining > 0);

            }
        }

        class Retreat : State
        {
            Mode mode;
            double turnRadius, travelDistance, startTime;
            public override void Start(Cooks newRobot)
            {
                mode = Mode.Retreat;
                robot = newRobot;
                turnRadius = 45;
                startTime = robot.Time;
            }

            public override void Update(ref ScannedRobotEvent scan, ref HitByBulletEvent hit)
            {
                Console.WriteLine("Retreat!!");
                if (scan != null)
                    robot.FireShots(scan.Distance);

                robot.SetTurnLeft(turnRadius);
                robot.SetBack(200);

                robot.Execute();

                turnRadius *= -1;

                if (robot.Time - startTime >= 3)
                    robot.fsm.Transition(mode, Mode.Intel);
            }

            public override void Exit()
            {

            }
        }

        class Intel : State
        {
            Mode mode;
            public override void Start(Cooks newRobot)
            {
                mode = Mode.Intel;
                robot = newRobot;

                Console.WriteLine("Scanning...");
                robot.SetTurnRadarRight(double.PositiveInfinity);
            }

            public override void Update(ref ScannedRobotEvent scan, ref HitByBulletEvent hit)
            {

                if (scan != null)
                {
                    Console.WriteLine("Found one!");
                    robot.SetTurnRight(scan.Bearing);
                    robot.SetTurnGunRight(scan.Bearing + (robot.Heading - robot.GunHeading));
                    robot.SetTurnRadarRight(-robot.RadarTurnRemaining);
                    robot.fsm.Transition(mode, Mode.Charge);
                }
            }

            public override void Exit()
            {

            }
        }

        

        //Idea for an enemy class: http://mark.random-article.com/weber/java/robocode/lesson4.html
        //I did not directly view steps for making this class
        //I only saw that they applied a class to predict and track enemies as described in the link
        internal struct OtherBot
        {
            String name;
            double velocity, x, y, futureX, futureY;
            double energy;
            double heading, bearing, distance;

            public String Name() { return name; }
            public double GetX() { return x; }
            public double GetY() { return y; }
            public double GetDistance() { return distance; }
            public double GetHeading() { return heading; }
            public double GetBearing() { return bearing; }

            public void Update(ScannedRobotEvent e, Cooks myRobot)
            {
                name = e.Name;
                velocity = e.Velocity;
                distance = e.Distance;
                
                //Update angles for trig (sub 90 degrees since 0 on unit circle = 90 in robot bearings)
                double theta = Utils.NormalAbsoluteAngle(e.Bearing) - Math.PI/2;

                double xDist = Math.Cos(theta) * e.Distance;
                double yDist = Math.Sin(theta) * e.Distance;
                x = myRobot.X + xDist;
                y = myRobot.Y + yDist;

                energy = e.Energy;
                heading = e.Heading;
                bearing = e.Bearing;
            }

            public double PredictX(double time)
            {
                double predictX = -1;
                //Assuming straight linear path
                double travelDistX = time * velocity * Math.Cos(Utils.NormalAbsoluteAngle(heading));

                predictX = x + travelDistX;

                return predictX;
            }

            public double PredictY(double time)
            {
                double predictY = -1;
                //Assuming straight linear path
                double travelDist = time * velocity * Math.Sin(Utils.NormalAbsoluteAngle(heading));

                predictY = y + travelDist;

                return predictY;
            }
        }


        public override void Run()
        {
            fsm = new FSM { robot = this };
            fsm.Init();
            rand = new Random();
            IsAdjustRadarForRobotTurn = true;
            IsAdjustGunForRobotTurn = true;
            target = new OtherBot();
            counterThreshold = 3;
            shotCounter = counterThreshold;
            hitCounter = 0;

            while (true)
            {
                //SetTurnRadarRight(double.PositiveInfinity);
                //TakeAction();
                //Execute();
                fsm.Update();
            }
        }

        public void TakeAction()
        {
            
            if (mode == Mode.Intel)
            {
                //TurnRight(15);
                
                SetTurnRadarRight(double.PositiveInfinity);
                //SetTurnLeft(45);
                //SetAhead(200);
                //Execute();
            }
            else if (mode == Mode.Charge)
            {

                //Move toward bot
                //SetAhead(target.getDistance());
                /*
                 * //Rams and shoots certain number of times
                int dodgeAngle = 45;
                int dodge = rand.Next(75, 150);
                while (shotCounter > 0)
                {
                    if (target.GetDistance() > 50)
                        SetAhead(target.GetDistance());

                    //Start dodging
                    if (DistanceRemaining == 0)
                    {
                        SetTurnRight(target.GetBearing() + 90);
                        Ahead(dodge);
                    }

                    FireShots(target.GetDistance());
                    Execute();
                    dodgeAngle *= -1;
                    dodge *= -1;
                    shotCounter--;
                }
                
                //Scan again
                mode = Mode.Intel;
                */

                //double currentDistance = target.GetDistance();
                SetAhead(target.GetDistance());
                Execute();

                while (DistanceRemaining > 20)
                    FireShots(target.GetDistance());

                //Switch to dodge when done
                TurnRight(90);
                mode = Mode.Dodge;
            }
            else if (mode == Mode.Dodge)
            {
                int dodgeAngle = 45;
                int dodge = rand.Next(75, 150);

                SetAhead(dodge);
                FireShots(target.GetDistance());

                Execute();
            }
            else if (mode == Mode.Retreat)
            {
                SetTurnLeft(45);
                SetBack(200);

                //Reset hit counter
                hitCounter = 0;

                //Scan again
                mode = Mode.Intel;
            }

        }

        public double GetDistance(double targetX, double targetY)
        {
            double xDist = targetX - this.X;
            double yDist = targetY - this.Y;
            double distance = Math.Sqrt(Math.Pow(xDist, 2) +  Math.Pow(yDist, 2));
            return distance;
        }

        public void FireShots(double distance)
        {
            double power;

            //Use more power when closer
            if (distance < 100)
                power = (Rules.MAX_BULLET_POWER);
            else if (distance < 200)
                power = (Rules.MAX_BULLET_POWER / 3 * 2);
            else if (distance < 400)
                power = (Rules.MAX_BULLET_POWER / 2);
            else if (distance < 600)
                power = (Rules.MAX_BULLET_POWER / 3);
            else
                power = (Rules.MAX_BULLET_POWER / 4);

            if (power != 0)
                SetFire(power);

            shotCounter++;

            Execute();

            /*
            //Guess Future enemy position
            double travelTime = target.GetDistance() / (20 - (power * 3));
            double futureX = target.PredictX(travelTime);
            double futureY = target.PredictY(travelTime);

            double shotAngle = Math.Atan((futureY - Y) / (futureX - X));
            TurnGunRight(shotAngle);
            SetFire(power);

            Execute();
            */
        }

        public override void OnScannedRobot(ScannedRobotEvent e)
        {
            if (IsTeammate(e.Name))
                return;
            else
            {
                fsm.EnqueueScan(e);

                //Update target info, assign target if there isn't one or replace with closer target
                if (e.Name == target.Name() || target.Name() == null || e.Distance < target.GetDistance())
                {
                    target.Update(e, this);
                    SetTurnRadarRight(-RadarTurnRemaining);
                }
                
                //Face enemy (skip if in defensive mode)
                /*
                Mode currentMode = fsm.State();
                if (currentMode == Mode.Charge)
                    SetTurnRight(e.Bearing);
                */
                //Aim
                //SetTurnGunRight(e.Bearing + (Heading - GunHeading));


                //Stop radar
                //SetTurnRadarRight(-RadarTurnRemaining);

                //SetAhead(e.Distance);


                //Determine strength of bullet (only fire when within 400 meters)
                /*
                double power = 1;
                if (e.Distance < 100)
                    power = (Rules.MAX_BULLET_POWER);
                else if (e.Distance < 200)
                    power = (Rules.MAX_BULLET_POWER / 3 * 2);
                else if (e.Distance < 300)
                    power = (Rules.MAX_BULLET_POWER / 2);
                else if (e.Distance < 400)
                    power = (Rules.MAX_BULLET_POWER / 3);
                else
                    power = 0;

                if (power != 0)
                    SetFire(power);
                */

                /*
                //Set aggressive mode and refresh shotCounter (ignore if in defensive mode)
                if (currentMode == Mode.Intel)
                {
                    fsm.Transition(currentMode, Mode.Charge);
                }
                */

                /*Aim and Fire
                double velocity = e.Velocity;
                double bulletSpeed = 20 - (3 * power);
                double travelTime = e.Distance / bulletSpeed;

                double angle = 0;
                if (velocity > 0)
                {

                }
                else if (velocity < 0)
                {

                }
                //Lead shot
                SetTurnGunRight(angle);

                //Fire
                SetFire(power);

                //Realign gun
                SetTurnGunRight(-angle);

                //Move toward enemy
                SetAhead(e.Distance / 0.75);

                //Fire straight shot
                SetFire(power);
                */

                /*
                SetTurnRadarRight(0);
                mode = Mode.Aggressive;
                Fire(1);

                double displacement = e.Distance;

                TurnRight(e.Bearing + 90);
                TurnGunLeft(90);
                */
            }
        }

        public override void OnHitByBullet(HitByBulletEvent e)
        {
            fsm.EnqueueHit(e);
            hitCounter++;

            //Go defensive if 3 shots have hit, shot was powerful or energy is low
            //if (hitCounter > counterThreshold || e.Bullet.Power > Rules.MAX_BULLET_POWER / 3 * 2 || Energy < 50)
                //mode = Mode.Retreat;
            //Back(100);
        }

        public override void OnHitRobot(HitRobotEvent e)
        {
            if (e.Name == target.Name())
                ramCount++;
        }
    }
}
