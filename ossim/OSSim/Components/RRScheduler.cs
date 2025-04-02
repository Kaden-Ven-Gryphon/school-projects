using System;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using System.Text;
using System.Threading.Tasks;

namespace OSSim.Components
{
    // child class for RR scheduling method which inherits from the scheduler abstract class
    public class RRScheduler : Scheduler
    {
        public class TimeQuantumEventArg : EventArgs
        {
            public int newTimeQuantum;
        }
        public static event EventHandler<TimeQuantumEventArg> TimeQuantumUpdated;

        private static int timeQuantum = 5;
        public static int TimeQuantum {
            get
            {
                return timeQuantum;
            }
            set
            {
                timeQuantum = value;
                TimeQuantumUpdated?.Invoke(null, new TimeQuantumEventArg
                {
                    newTimeQuantum = timeQuantum
                });
            }
        }

        public override void OnProcessComplete(object sender, Process p)
        {
            if (sender is CPU)
            {
                lock (sender)
                {
                    // if typecast sender to cpu succeeds
                    if (sender is CPU cpu)
                    {
                        // If the process is not entirely complete, add it to the end of the process list
                        if (p.ServiceTime > 0)
                        {
                            //p.ServiceTime -= timeQuantum;
                            // Make sure the service time is not negative or 0 before adding it
                            if (p.ServiceTime > 0)
                            {
                                if (p.ServiceTime <= timeQuantum)
                                    p.Priority = 2;
                                ProcessList.Add(p);
                                // only need to add the single process, not the entire process list
                                var proclist = new List<Process>();
                                proclist.Add(p);
                                OnProcessListUpdated(this, new ProcessEventArgs { added = proclist });
                            } 
                        }
                        // Otherwise, the process is completed and can be added to the data table.
                        else
                        {
                            Debug.WriteLine("RRProc Complete");
                            // Restore the initial service time for data calculations
                            p.ServiceTime = p.StaticServiceTime;
                            p.StaticServiceTime = 0;
                            p.Priority = 2;
                            ProcessesFinished++;
                            TotalnTAT = TotalnTAT + p.NTAT;
                        }
                        var next = NextProcess;
                        // If there are more processes to run
                        if (next != null)
                        {
                            // Set the process' time to run equal to the input time quantum
                            if (next.ServiceTime > timeQuantum)
                                next.TimeRemaining = timeQuantum;
                            else
                            {
                                next.Priority = 2;
                                // if the time it takes to run the process is less than the time quantum, add the difference. 
                                // all processes will have an equal time slice.
                                next.TimeRemaining = next.ServiceTime;
                            }
                            cpu.CurrentProcess = next;
                            cpu.ProcessComplete = false;
                        }
                        else
                        {
                            cpu.CurrentProcess = null;
                        }
                    }
                }
            }
        }
    }
}
