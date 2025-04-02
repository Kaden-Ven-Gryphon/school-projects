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

namespace OSSim.UI
{
    
    // Table containing stats on finished processes. 
    // Dynamically adds a row when a process completes.
    public partial class DataTable : UserControl
    {
        public DataTable()
        {
            InitializeComponent();
            foreach(var ctrl in HeaderPanel.Controls)
            {
                var lbl = (ctrl as Label);
                lbl.BackColor = Color.Gray;
                // Show text when mouse over
                lbl.MouseHover += (object sender, EventArgs e) =>
                {
                    Tip.Show(lbl.Text, lbl);
                };
            }
        }

        // Updates stats table when process finishes
        public void OnProcessComplete(object sender, Process p)
        {
            if (p.Priority == 2)
            {
                AddItemPtr func = AddItem;
                BeginInvoke(func, new object[] { p });
            }
        }


        // Function pointer to run it on the UI thread
        public delegate void AddItemPtr(Process process);


        // Static tooltip for use on the table
        private static ToolTip Tip = new();

        // Generate a label for this component, each cell in the table
        Label LabelFactory(string text)
        {
            var lbl = new Label()
            {
                Text = text,
                Width = LblProcName.Width,
                Height = Convert.ToInt32(LblProcName.Height * 1.50),
                Margin = LblProcName.Margin,
                Padding = LblProcName.Padding,
                BackColor = Color.White,
                
             
            };
            lbl.Padding = new Padding(10);
            // All have the same hover event
            lbl.MouseHover += (object sender, EventArgs e) =>
            {
                Tip.Show(lbl.Text, lbl);
            };
            return lbl;
        }
        
        
        //Function to add lines to the Data Table
        public void AddItem(Process process)
        {
            //Create a layout for the new line
            var rowPanel = new TableLayoutPanel();
            dataTablePanelRows.Controls.Add(rowPanel);
            // Set properties for row
            rowPanel.Dock = DockStyle.Top;
            rowPanel.ColumnCount = 6;
            rowPanel.RowCount = 1;
            rowPanel.AutoSize = true;
            rowPanel.Margin = HeaderPanel.Margin;
            //rowPanel.Margin = new Padding(0,0,0,0);
            
            //Get a reference to the previous existing row
            
            //Increase panel rows count by one
            dataTablePanelRows.RowCount++;
            // Add labels
            rowPanel.Controls.Add(LabelFactory(process.ProcessID),              0, rowPanel.RowCount - 1);
            rowPanel.Controls.Add(LabelFactory(process.Arrivaltime.ToString()), 1, rowPanel.RowCount - 1);
            rowPanel.Controls.Add(LabelFactory(process.ServiceTime.ToString()), 2, rowPanel.RowCount - 1);
            rowPanel.Controls.Add(LabelFactory(process.FinishTime.ToString()),  3, rowPanel.RowCount - 1);
            rowPanel.Controls.Add(LabelFactory(process.TAT.ToString()),         4, rowPanel.RowCount - 1);
            rowPanel.Controls.Add(LabelFactory(process.NTAT.ToString("0.##")),  5, rowPanel.RowCount - 1);
            //Add the row
        }


    }
}
