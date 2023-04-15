using Robocode;
using System;
using System.ComponentModel;
using System.Drawing.Printing;

namespace CAP4053.Student
{
    public class Cooks : TeamRobot
    {
        Mode mode = Mode.Intel;
        enum Mode
        {
            Aggressive, Defensive, Intel
        }

        

        public override void Run()
        {
            IsAdjustGunForRobotTurn = true;
            IsAdjustRadarForRobotTurn = true;
            while (true)
            {
                //Check for hits
                takeAction();
            }
        }

        public void takeAction()
        {

            TurnGunRight(360);
            if (mode == Mode.Intel)
            {
                //TurnGunRight(360);
                //TurnRight(15);
            }
            else if (mode == Mode.Aggressive)
            {
                Fire(Rules.MAX_BULLET_POWER);
            }
            else if (mode == Mode.Defensive)
            {
                Back(200);
                mode = Mode.Intel;
            }
        }

        public override void OnScannedRobot(ScannedRobotEvent e)
        {
            if (IsTeammate(e.Name))
                return;
            else
            {
                //mode = Mode.Aggressive;
                Fire(1);

                double displacement = e.Distance;
                TurnRight(e.Bearing);
                SetAhead(e.Distance/2);
            }
        }

        public override void OnBulletMissed(BulletMissedEvent e)
        {
            mode = Mode.Intel;
        }

        public override void OnHitByBullet(HitByBulletEvent e)
        {
            //TurnGunRight(e.Bearing);
            if (Energy < 50)
                mode = Mode.Defensive;
        }
    }
}
