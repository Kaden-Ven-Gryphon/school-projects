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
// File displaying the throughput
namespace OSSim.UI
{
    public partial class ThroughputDisplay : UserControl
    {
        public ThroughputDisplay()
        {
            InitializeComponent();
            Throughput = 0;
        }

#nullable enable
        // Used to read elapsed time
        public Scheduler? scheduler;

        // Formats throughput as text in display

        public decimal Throughput {
            set { LblThroughput.Text = "Current Average nTAT: " + value.ToString("0.###"); }
        }


        // Updates every time the system clock updates
        public void OnTick(object sender, System.Timers.ElapsedEventArgs e)
        {
            
            if (scheduler?.ElapsedTime > 0)
            {
                try
                {
                    // Runs code in the lambda inside the UI thread.
                    // Equivalent to declaring a function pointer type(delegate) to the lambda and then setting such a pointer to the lambda
                    //      
                    BeginInvoke(new Action(() =>
                    {
                        if (scheduler.ProcessesFinished > 0)
                        {
                            Throughput = Convert.ToDecimal(scheduler.TotalnTAT) / scheduler.ProcessesFinished;
                        }
                        else Throughput = 0;
                    }));
                }
                catch(Exception)
                {
                    //Ignore exception, only occurs if timer starts before window loads
                }
            }
        }
    }
}
