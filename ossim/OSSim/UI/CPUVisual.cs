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
// Yellow CPU Visual, shows process executing and time left
namespace OSSim.UI
{
    public partial class CPUVisual : UserControl
    {
        public CPUVisual()
        {
            InitializeComponent();
        }
        // Number id for cpu
        public string CPUNumber
        {
            set { CPUNumberLabel.Text = "CPU " + value; }
        }
        // Current executing process
        public string ExecutingProcess
        {
            set { ExecLabel.Text = "Exec: " + value; }
        }
        // Service Time left to work on process
        public string TimeRemaining
        {
            set { TimeRemainingLabel.Text = "Time Remaining: " + value; }
        }

        // Whenever the CPU tick occurs.
        // Runs Update UI on the UI thread, but uses Invoke which may not be available during startup/shutdown
        public void OnTick(object sender, CPU.Status e)
        {
            var args = new object[] { e };
            // Wrap update method to run it on UI thread. C# has protections to prevent other threads from accessing the UI.
            try
            {
               
                OnUpdate update = UpdateUI;
                BeginInvoke(update, args);
            }
            catch (System.InvalidOperationException)
            {
                // Ignore exceptions, these occur from ticks when the UI is being created.
            }
        }

        // Wrapper to map the update to the UI thread.
        delegate void OnUpdate(CPU.Status e);
        // Put information from CPU onto this element.
        public void UpdateUI(CPU.Status e){
            ExecutingProcess = e.Exec;
            TimeRemaining = e.TicksLeft;
        }

       

    }
}
