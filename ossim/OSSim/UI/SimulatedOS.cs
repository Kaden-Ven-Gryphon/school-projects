using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using OSSim.Components;
// Segmentation for separate task schedulers, basically a simplified clone of the MainWindow from Phase II to be reproduced multiple times
namespace OSSim.UI
{
    public partial class SimulatedOS : UserControl
    {
        // Physical backend of cpu, cpu variables represent graphical representations of cpus

        public SimulatedOS()
        {
            InitializeComponent();
        }

        // Manual Backend function, to be called by the main window.

        // Core -> CPU sent to this visual, can be preinitialized externally
        // Scheduler -> Passed in scheduler, pick an interface that works for this object (HRRN or RR)
        // Label -> What to display the cpu as
        public void InitBackend(CPU core, Scheduler scheduler, string cpulabel)
        {

            //Proc table init
            scheduler.ProcessListUpdated += proctable.SafeUpdateTable;

            // CPU init
            core.Updated += cpu.OnTick;
            cpu.CPUNumber = cpulabel;
                // Give the CPU an process if there's one ready
            var proc = scheduler.NextProcess;
            if (proc != null)
            {
                if (scheduler is RRScheduler)
                {
                    core.RunProcRR(proc, RRScheduler.TimeQuantum);
                    RRScheduler.TimeQuantumUpdated += core.OnTimeQuantumUpdated;
                }
                    
                else
                    core.RunProcess(proc);
            }
            // Update the table when a process completes
            // TODO: bind to CPU
            core.OnProcessComplete += dataTable1.OnProcessComplete;
            // Update the process throughput every tick
            core.Updated += cpu.OnTick;
            core.OnProcessComplete += scheduler.OnProcessComplete;
            CPU.AddToTick(throughputDisplay1.OnTick);

            // Clear the table without the ScheduledCPU GUI component being  recognized
            proctable.UpdateTable(scheduler, new Scheduler.ProcessEventArgs() { added = scheduler.ProcessesInQueue });

            cpu.UpdateUI(core.CurrentStatus);
            // Link the clock on the scheduler to the throughput display
            throughputDisplay1.scheduler = scheduler;
            
        }
    }
}
