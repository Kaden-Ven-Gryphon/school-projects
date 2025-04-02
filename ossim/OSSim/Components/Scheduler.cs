//**************************************************************************
// Filename: Scheduler.cs
// Purpose: Holds common elements to be used by the two scheduling subclasses which
// implement round robin and HRRN. Reads Process data from CSV file to be stored as a
// Process object.
//**************************************************************************
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows.Forms;
namespace OSSim.Components
{
    // Abstract class which implements common functionality for each of the different
    // scheduling algorithms
    public abstract class Scheduler
    {
        // Event to trigger UI update on proc table
        public event EventHandler<ProcessEventArgs> ProcessListUpdated;

        // Object sent to events when the process list was updated.
        public class ProcessEventArgs : EventArgs
        {
            public IEnumerable<Process> added;
            public IEnumerable<Process> removed;
        }
        protected List<Process> ProcessList { get; set; }
        protected List<Process> ProcsWaiting { get; set; }

        public int ElapsedTime { get; set; }
        public int ProcessesFinished { get; set; }
        public decimal TotalnTAT { get; set; }
        // Allow other objects to see a copy of the processes available.
        public IEnumerable<Process> ProcessesInQueue { get { return ProcessList; } }

        /// <summary>
        /// Constructor, initializes list data structure and populates the data from the CSV file
        /// </summary>
        public Scheduler(string file = "")
        {
            ProcessList = new List<Process>();
            ProcsWaiting = new List<Process>();
            if (file != "")
            {
                ReadProcesses(file);
            }
        }


        // Schedule a new process since one completed
        // Implementation will vary based on which scheduling algorithm is used

        public abstract void OnProcessComplete(object sender, Process p);

        //to be used by the child classes to envoke ProcessListUpdated
        public void OnProcessListUpdated(object sender, ProcessEventArgs e)
        {
            ProcessListUpdated?.Invoke(sender, e);
        }



        // Gets the next process in the queue, popping it off.
        // With an empty queue null is returned
#nullable enable
        public virtual Process? NextProcess
        {
            get
            {
                if (ProcessList.Count <= 0)
                {
                    return null;
                }

                var ret = ProcessList[0];
                ProcessList.RemoveAt(0);
                // List changed, only one thing removed
                ProcessListUpdated?.Invoke(this, new ProcessEventArgs()
                {
                    removed = new List<Process>() { ret }
                });
                return ret;
            }
        }


        // Look to update process list at each tick
        // Called once every time the timer completes, regardless of the number of cpus
        public void OnClockTick(Object source, System.Timers.ElapsedEventArgs e)
        {
            if (CPU.Paused)
            {
                // Don't do anything if the simulation is paused
                return;
            }

            //increment overall elapsed time, which is used to determine process arrival time.
            ElapsedTime++;
            OnProcArrival(e);

        }

        // Each time the cpu timer elapses, check if a process' arrival time corresponds. If so, the process can be added
        // to the process list and the UI process table is updated.
        public void OnProcArrival(System.Timers.ElapsedEventArgs e)
        {
            // Buffer for new processes
            var ProcList = new List<Process>();
            if (ProcsWaiting.Count <= 0)
                return;
            bool updated = false;
            // Add all procs ready at this time
            for (int c = 0; c < ProcsWaiting.Count; c++)
            {
                if (ElapsedTime >= ProcsWaiting[c].Arrivaltime)
                {
                    ProcList.Add(ProcsWaiting[c]);
                    ProcsWaiting.RemoveAt(c);
                    c--;
                    updated = true;
                    Debug.WriteLine("New process arrived at " + ElapsedTime);

                }
            }
            // Finalize submission of new procs, only update on change
            if (updated)
            {
                ProcessList.AddRange(ProcList);
                if (ProcessList.Count != 1)
                    ProcessListUpdated.Invoke(this, new ProcessEventArgs { added = ProcList });
            }

        }
        /// <summary>
        /// Parses data from a CSV file
        /// Data file sample.csv stored in OSSim\bin\Debug\net5.0-windows
        /// </summary>
        public void ReadProcesses(string file)
        {
            // Temporary variable to hold data parsed from CSV
            string[] queue;
            try
            {
                // File contents
                string[] CSVLines = File.ReadAllLines(file);
                // Buffer for Processes
                var ProcList = new List<Process>();

                for (int i = 0; i < CSVLines.Length; i++)
                {
                    queue = CSVLines[i].Split(new String[] {","}, StringSplitOptions.RemoveEmptyEntries);
                    var process = new Process()
                    {
                        Arrivaltime = Convert.ToInt32(queue[0]),
                        ProcessID = queue[1],
                        ServiceTime = Convert.ToInt32(queue[2]),
                        StaticServiceTime = Convert.ToInt32(queue[2]),
                        Priority = Convert.ToInt32(queue[3])
                    };
                    //Check whether or not the process arrives immediately - if it does not, add it to a secondary waiting list.
                    if (process.Arrivaltime != 0)
                    {
                        ProcsWaiting.Add(process);
                    }
                    else
                    {
                        ProcList.Add(process);
                    }
                }
                // New processes were added, invoke event.
                ProcessList.AddRange(ProcList);
                ProcessListUpdated?.Invoke(this, new ProcessEventArgs { added = ProcList.ToList() });
            }
            catch (FileNotFoundException ex)
            {
                Debug.WriteLine("Error: File not found - ", ex);
                MessageBox.Show("Error: Could not read process list from " + file);

            }
            catch (DirectoryNotFoundException ex)
            {
                Debug.WriteLine("Error: Directory not found - ", ex);
            }
        }
    }
}