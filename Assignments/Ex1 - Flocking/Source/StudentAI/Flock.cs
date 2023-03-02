using System.Collections.Generic;
using System.Security.Cryptography.X509Certificates;
using System.Xml.Schema;
using AI.SteeringBehaviors.Core;

namespace AI.SteeringBehaviors.StudentAI
{
    public class Flock
    {
        public float AlignmentStrength { get; set; }
        public float CohesionStrength { get; set; }
        public float SeparationStrength { get; set; }
        public List<MovingObject> Boids { get; protected set; }
        public Vector3 AveragePosition { get; set; }
        protected Vector3 AverageForward { get; set; }
        public float FlockRadius { get; set; }

        #region Ex1
        //Helper functions
        private Vector3 calcAvgPos(List<MovingObject> boids)
        {
            Vector3 avgPos = new Vector3();
            float x = 0, y = 0, z = 0;
            for (int i = 0; i < boids.Count; i++)
            {
                x += boids[i].Position.X;
                y += boids[i].Position.Y;
                z += boids[i].Position.Z;
            }
            avgPos.X = x / (float)boids.Count;
            avgPos.Y = y / (float)boids.Count;
            avgPos.Z = z / (float)boids.Count;

            return avgPos;
        }
        private Vector3 calcAvgFrwd(List<MovingObject> boids)
        {
            Vector3 avgFrwd = new Vector3();
            float x = 0, y = 0, z = 0;
            for (int i = 0; i < boids.Count; i++)
            {
                x += boids[i].Velocity.X;
                y += boids[i].Velocity.Y;
                z += boids[i].Velocity.Z;
            }
            avgFrwd.X = x / (float)boids.Count;
            avgFrwd.Y = y / (float)boids.Count;
            avgFrwd.Z = z / (float)boids.Count;

            return avgFrwd;
        }
        private Vector3 calcAlignAccel(MovingObject boid)
        {
            Vector3 accel = new Vector3();
            accel = AverageForward / boid.MaxSpeed;

            if (accel.Length > 1)
                accel.Normalize();

            return accel * AlignmentStrength;
        }
        private Vector3 calcCohesionAccel(MovingObject boid)
        {
            Vector3 accel = new Vector3();
            accel = AveragePosition - boid.Position;
            float dist = accel.Length;
            accel.Normalize();

            if (dist < FlockRadius)
                accel *= dist / FlockRadius;

            return accel * CohesionStrength;
        }
        private Vector3 calcSeparationAccel(MovingObject boid)
        {
            Vector3 sum = new Vector3();
            
            for (int i = 0; i < Boids.Count; i++)
            {
                if (boid == Boids[i])
                    continue;

                Vector3 accel = new Vector3();
                accel = boid.Position - Boids[i].Position;
                float dist = accel.Length;
                float safeDist = boid.SafeRadius + Boids[i].SafeRadius;

                if (dist < safeDist)
                {
                    accel.Normalize();
                    accel *= (safeDist - dist) / safeDist;
                    sum += accel;
                }

            }

            if (sum.Length > 1)
                sum.Normalize();

            return sum * SeparationStrength;
        }
        public Flock()
        {
            Boids = new List<MovingObject>();

            AverageForward = new Vector3();
            AveragePosition = new Vector3();

            AlignmentStrength = 1;
            CohesionStrength = 1;
            SeparationStrength = 1;
            FlockRadius = 10;
        }

        public virtual void Update(float deltaTime)
        {
            // Update goes here
            AveragePosition = calcAvgPos(Boids);
            AverageForward = calcAvgFrwd(Boids);

            for (int i = 0; i < Boids.Count; i++)
            {
                Vector3 accel = calcAlignAccel(Boids[i]);
                accel += calcCohesionAccel(Boids[i]);
                accel += calcSeparationAccel(Boids[i]);
                accel *= Boids[i].MaxSpeed * deltaTime;
                Boids[i].Velocity += accel;

                if (Boids[i].Velocity.Length > Boids[i].MaxSpeed)
                {
                    Boids[i].Velocity.Normalize();
                    Boids[i].Velocity *= Boids[i].MaxSpeed;
                }

                Boids[i].Update(deltaTime);
            }
        }
        #endregion

    }
}
