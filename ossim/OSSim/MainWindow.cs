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

namespace OSSim
{
    // Main Window, hold the entirety of the gui
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();
            InitBackgroundComponents();
        }
        // Size of the main window, used to scale components correctly
     

        // Initialize the tasks running in the background of this GUI.
        public void InitBackgroundComponents()
        {
            // Initialize CPU 1 to run with HRRN scheduler
            Scheduler HRRNScheduler = new HRRNScheduler();
            CPU HRRNCore = new CPU(HRRNScheduler);
            string file = System.Configuration.ConfigurationManager.AppSettings["fileName"];
            HRRNScheduler.ReadProcesses(file);
            // Set visuals/
            Sim1.InitBackend(HRRNCore, HRRNScheduler, "1 (HRRN)");
            // Set initial processes

            // Update the process throughput every tick
            CPU.AddToTick(HRRNScheduler.OnClockTick);
            CPU.AddToTick(OnTick);

            Scheduler = HRRNScheduler;


            // Initialize CPU 2 to run with RR scheduler
            Scheduler RRScheduler = new RRScheduler();
            CPU RRCore = new CPU(RRScheduler);
            RRScheduler.ReadProcesses(file);

            Sim2.InitBackend(RRCore, RRScheduler, "2 (RR)");

            CPU.AddToTick(RRScheduler.OnClockTick);
            CPU.AddToTick(OnTick);

            Scheduler = RRScheduler;
            
        }
        // Used to keep time
        private Scheduler Scheduler;

        private void BtnPause_Click(object sender, EventArgs e)
        {
            CPU.Paused = true;
            LblSimulationStatus.Text = "System Paused";
        }

        private void BtnStart_Click(object sender, EventArgs e)
        {
            CPU.Paused = false;
            LblSimulationStatus.Text = "System Running";
            // If the RR Time slice is not specified, set default to 5
            if (String.IsNullOrEmpty(RRTextBox.Text))
                RRScheduler.TimeQuantum = 5;
        }

        //Change Time unit whenever the text changes. Pause the system in addition, to prevent oddities
        private void TxtInterval_TextChanged(object sender, EventArgs e)
        {
            if (int.TryParse(txtInterval.Text, out int field) && field != 0)
            {
                BtnPause_Click(sender, e); // Pause the system
                try
                {
                    CPU.TimeUnit = field;
                }
                catch (ArgumentOutOfRangeException)
                {
                    txtInterval.Text = "1000"; // This will trigger another event
                }
            }
        }
        
        //Change the Time Quantum for the Round Robin scheduling algorithm. 
        private void RRTextBox_TextChanged(object sender, EventArgs e)
        {
            if (int.TryParse(RRTextBox.Text, out int field) && field != 0)
            {
                
                BtnPause_Click(sender, e);
                try
                {
                    if (field > 0)
                    {
                        RRScheduler.TimeQuantum = field;
                        
                    }
                    else
                        RRScheduler.TimeQuantum = 5; // use a default value is the field is invalid
                }
                catch (ArgumentOutOfRangeException)
                {
                    // if there was an error, use a default value
                    RRScheduler.TimeQuantum = 5;
                    RRTextBox.Text = "5";
                }
            }
        }

        private void OnTick(object sender, System.Timers.ElapsedEventArgs e)
        {
            // Ignore exceptions from before main window is loaded
            try
            {
                LblElapsed.BeginInvoke(new Action(() =>
                {
                    LblElapsed.Text = "Time Elapsed: " +  Scheduler.ElapsedTime.ToString();
                }));
            }
            catch(System.InvalidOperationException)
            {

            }
        }

    }
}
