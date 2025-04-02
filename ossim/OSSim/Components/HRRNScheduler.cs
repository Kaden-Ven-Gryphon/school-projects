using System;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using System.Text;
using System.Threading.Tasks;

namespace OSSim.Components
{
    // child class for HRRN scheduling method which inherits from the scheduler abstract class
    //works the same as before except it uses NextHRRNProcess instead of nextProcess
    public class HRRNScheduler : Scheduler
    {
#nullable enable
        public override Process? NextProcess
        {
            get
            {
                // if there are processes remaining
                if (ProcessList.Count > 0)
                {
                    float responseRatio = 0;
                    int bestIndex = 0;
                    Process ret;
                    //find the proccess is the highest response ratio
                    for (int i = 0; i < ProcessList.Count; i++)
                    {
                        ret = ProcessList[i];
                        float rr = 999;

                        // if service timer is some how zero, avoid devideing by zero and just leave rr as 999
                        if (ret.ServiceTime != 0)
                        {
                            rr = (ret.ServiceTime + (ElapsedTime - ret.Arrivaltime)) / ret.ServiceTime;
                        }

                        // if a higher rr is found keep track of it
                        if (rr > responseRatio)
                        {
                            responseRatio = rr;
                            bestIndex = i;
                        }
                    }

                    // the process to be returned
                    ret = ProcessList[bestIndex];
                    ret.Priority = 2;


                    // remove it from the list
                    ProcessList.RemoveAt(bestIndex);

                    // List changed, only one thing removed

                    OnProcessListUpdated(this, new ProcessEventArgs()
                    {
                        removed = new List<Process>() { ret }
                    });
                    // Ensure the process runs for its entire duration
                    ret.TimeRemaining = ret.ServiceTime;
                    return ret;
                }
                // return null if no process remaining
                else return null;
            }
        }
        public override void OnProcessComplete(object sender, Process p)
        {
            if (sender is CPU)
            {
                lock (sender)
                {
                    Debug.WriteLine("Proc Complete");
                    ProcessesFinished++;
                    TotalnTAT += p.NTAT;
                    // if typecast sender to cpu succeeds
                    if (sender is CPU cpu)
                    {
                        // get the HRRN process
                        var next = NextProcess;
                        // If there are more processes to run
                        if (next != null)
                        {
                            next.TimeRemaining = next.ServiceTime;
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
