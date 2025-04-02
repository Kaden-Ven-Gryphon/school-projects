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

// Displays processes not currently running
namespace OSSim.UI
{
    public partial class ProcessTable : UserControl
    {
        // Table of processes
        protected List<Proc> Processes { get; set; }
        
        // UI for Process.
        public struct Proc {
            public Label Name;
            public Label TimeCost;
        }

        // Constructor for the process table
        public ProcessTable()
        {
            InitializeComponent();
            Processes = new List<Proc>();
            LblProcName.Padding = new Padding() { All = 10 };
        }


        // Wrap update function to ensure it only runs on the UI thread.
        public void SafeUpdateTable(object sender, Scheduler.ProcessEventArgs e)
        {
            try
            {
                UpdatePtr func = UpdateTable;
                BeginInvoke(func, new object[] { sender, e });
            }catch (System.InvalidOperationException)
            {
                // Ignore exceptions from UI loading 
            }
        }

        // Function pointer type.
        public delegate void UpdatePtr(object sender, Scheduler.ProcessEventArgs e);
        // Updates Process Queue Visual
        public void UpdateTable(object sender, Scheduler.ProcessEventArgs e)
        {
            var bufferPanel = PnlTable;
            var parent = PnlTable.Parent;
            bufferPanel.Parent = null;
            // Remove old procs
            if (e.removed != null)
            {
                foreach (var proc in e.removed)
                {
                    RemoveProc(bufferPanel,proc.ProcessID, proc.ServiceTime);
                }
            }
            // Add new procs
            if (e.added != null)
            {
                foreach (var proc in e.added)
                {
                    AddProc(bufferPanel,proc.ProcessID, proc.ServiceTime);
                }
            }
            bufferPanel.Parent = parent;
            
        }

        // Clear all procs from this UI element.
        public void Clear()
        {
            foreach(Proc p in Processes)
            {
                PnlTable.Controls.Remove(p.Name);
                PnlTable.Controls.Remove(p.TimeCost);
            }
            Processes.Clear();
        }
       


        // Static tooltip for use on the table
        private static ToolTip Tip = new();

        // Adds a process to the table on this component.
        public void AddProc(TableLayoutPanel PnlTable,string name, int cost)
        {
            var lblSize = LblProcName.Size;
            var p = new Proc() {
                Name = new Label() {
                    Text = name,
                    Size = lblSize,
                },
                TimeCost = new Label() {
                    Text = cost.ToString(),
                    Size = lblSize
                }
            }; 
            p.Name.MouseHover +=  (object sender, EventArgs e) =>
            {
                Tip.Show(p.Name.Text, p.Name);
            };
            PnlTable.Controls.Add(p.Name);
            PnlTable.Controls.Add(p.TimeCost);
            Processes.Add(p);

        }

        /// <summary>
        /// Remove a process from the list.
        /// <paramref name="name"></paramref>: Name of the Process
        /// <paramref name="cost"></paramref>: If given, the time left on the process.
        /// </summary>
        public void RemoveProc(TableLayoutPanel PnlTable, string name, int cost = -1)
        {

            // Find the proc
            var p = Processes.Find((Proc p) => { 
                return p.Name.Text == name && (cost == -1 || p.TimeCost.Text == cost.ToString()); 
            });
            // Remove it. If not found, nothing happens
            PnlTable.Controls.Remove(p.Name);
            PnlTable.Controls.Remove(p.TimeCost);
            Processes.Remove(p);
        }


        

       
    }
}
