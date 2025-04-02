//**************************************************************************
// Filename: CPU.cs
// Purpose:  Background Simulation class for CPUs.
//**************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSSim.Components
{
    // an individual processor for the multiprocessor system
    public class CPU
    {
        // Flag for if all CPUs are running
        private static bool paused = true;
        public static bool Paused {
            get { return paused; }
            set { paused = value; }
        }
#nullable enable
        //when the current process is complete this is set to ture
        public bool ProcessComplete { get; set; }
        // Current Executing process, can be null
        public Process? CurrentProcess { get; set; }

        // Event that occurs whenever a process is complete. Returns the completed process.
        public event EventHandler<Process>? OnProcessComplete;

        public void OnTimeQuantumUpdated(object sender, RRScheduler.TimeQuantumEventArg e)
        {
            if(CurrentProcess != null)
            {
                CurrentProcess.TimeRemaining = e.newTimeQuantum;
                if(CurrentProcess.ServiceTime < e.newTimeQuantum)
                {
                    CurrentProcess.Priority = 2;

                }
            }
        }


        // Acts as a shared CPU clock
        private static readonly System.Timers.Timer timer;
        
        //the time in ms of a time unit for simulation
        public static double TimeUnit {
            get { return timer.Interval; }
            set {
                if (value > 0)
                {
                    timer.Interval = value;
                }else
                {
                    throw new ArgumentOutOfRangeException("TimeUnit", "Must be greater than 0");
                }
            }
        }
        // Allow additional hooks to the clock
        public static void AddToTick(System.Timers.ElapsedEventHandler hook)
        {
            timer.Elapsed += hook;
        }
        
        // Called on startup, initializes a unified clock for all CPUs.
        static CPU()
        {
            timer = new System.Timers.Timer
            {
                Enabled = true,
                Interval = 500,
                AutoReset = true                            
            };
        }


        private Scheduler Scheduler;

        public CPU(Scheduler s)                                //CONSTRUCTOR*****************************
        {
            ProcessComplete = false;                                        
            paused = true;                                      
            // Cause the CPU to simulate a clockTick whenever the timer elapses
            timer.Elapsed += ClockTick;
            // do nothing handler
            Updated = new EventHandler<Status>((o,s)=> { });
            Scheduler = s;

            
        }
        // Remove the CPU from the timer when destroyed
        ~CPU()
        {
            timer.Elapsed -= ClockTick;
        }
            
        //This sets up the timer and kicks off running the process
        public int RunProcess(Process proc)                                 //RUNPROCESS*************************************
        {
            CurrentProcess = proc;
            proc.TimeRemaining = proc.ServiceTime;
            proc.Priority = 2;

            return 0;                                           
        }
        //Used for the first process scheduled by the RR scheduler
        public int RunProcRR(Process proc, int quant)
        {
            CurrentProcess = proc;
            proc.TimeRemaining = 5; // use default value for the first process
            return 0;
        }

        // Information to feed to CPU GUI
        public class Status
        {
            public string Exec = "";
            public string TicksLeft = "";
        }
        // the current status of CPU, i.e. whether its executing or not and how long the process has left.
        public Status CurrentStatus
        {
            get
            {
                if(Scheduler is RRScheduler)
                {
                    return new Status()
                    {
                        Exec = (CurrentProcess == null || CurrentProcess.TimeRemaining <= 0) ? "idle" : CurrentProcess.ProcessID,
                        TicksLeft = (CurrentProcess == null || CurrentProcess.TimeRemaining <= 0) ? "n/a" : (CurrentProcess.ServiceTime).ToString()

                    };
                }
                else
                {
                    return new Status()
                    {
                        Exec = (CurrentProcess == null || CurrentProcess.TimeRemaining <= 0) ? "idle" : CurrentProcess.ProcessID,
                        TicksLeft = (CurrentProcess == null || CurrentProcess.TimeRemaining <= 0) ? "n/a" : CurrentProcess.TimeRemaining.ToString()

                    };
                }
                
            }
        }

        // Event to trigger whenever the CPU state changes.
        public event EventHandler<Status> Updated;

        //This is called every time the timer completes, for each cpu
        private void ClockTick(Object source, System.Timers.ElapsedEventArgs e)   //STEPPROCESS**************************************
        {
            // Only one tick can be active at a time for the CPU, prevents race conditions from a buildup of timer elapsed events, such as when debugging.
            lock (this)
            {
                // On paused, skip everything else.
                if (Paused) 
                {
                    // idle CPU.
                    return;
                }
                if (ProcessComplete || CurrentProcess == null)
                {
                    CurrentProcess = Scheduler.NextProcess;
                    ProcessComplete = false;
                }

                //if the process has been complete
                if (CurrentProcess?.TimeRemaining <= 1 || CurrentProcess?.ServiceTime <= 1)
                {
                    // Call complete event on change to complete.
                    CurrentProcess.TimeRemaining--;
                    if(Scheduler is RRScheduler)
                    {
                        CurrentProcess.ServiceTime = CurrentProcess.ServiceTime - 1;
                    }
                    // This call occurs before the clock tick registers, so add 1
                    CurrentProcess.FinishTime = Scheduler.ElapsedTime+1;
                    ProcessComplete = true;
                    this.OnProcessComplete?.Invoke(this, CurrentProcess); 
                    Updated?.Invoke(this, CurrentStatus);
                    return;
                }
                //decrement the service time, cpu did work 
                if(CurrentProcess != null)
                {
                    CurrentProcess.TimeRemaining--;
                    if(Scheduler is RRScheduler)
                    {
                        CurrentProcess.ServiceTime = CurrentProcess.ServiceTime - 1;
                    }
                }
                Updated?.Invoke(this, CurrentStatus);

            }
        }

        
        
    }
}
