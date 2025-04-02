
namespace OSSim.UI
{
    partial class SimulatedOS
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.proctable = new OSSim.UI.ProcessTable();
            this.cpu = new OSSim.UI.CPUVisual();
            this.dataTable1 = new OSSim.UI.DataTable();
            this.throughputDisplay1 = new OSSim.UI.ThroughputDisplay();
            this.SuspendLayout();
            // 
            // proctable
            // 
            this.proctable.Location = new System.Drawing.Point(0, 0);
            this.proctable.Margin = new System.Windows.Forms.Padding(0);
            this.proctable.Name = "proctable";
            this.proctable.Size = new System.Drawing.Size(431, 1171);
            this.proctable.TabIndex = 1;
            // 
            // cpu
            // 
            this.cpu.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.cpu.Location = new System.Drawing.Point(442, 13);
            this.cpu.Margin = new System.Windows.Forms.Padding(11, 13, 11, 13);
            this.cpu.Name = "cpu";
            this.cpu.Size = new System.Drawing.Size(373, 320);
            this.cpu.TabIndex = 2;
            // 
            // dataTable1
            // 
            this.dataTable1.AutoSize = true;
            this.dataTable1.Location = new System.Drawing.Point(436, 337);
            this.dataTable1.Margin = new System.Windows.Forms.Padding(9, 13, 9, 13);
            this.dataTable1.Name = "dataTable1";
            this.dataTable1.Size = new System.Drawing.Size(962, 834);
            this.dataTable1.TabIndex = 9;
            // 
            // throughputDisplay1
            // 
            this.throughputDisplay1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.throughputDisplay1.Location = new System.Drawing.Point(0, 1188);
            this.throughputDisplay1.Margin = new System.Windows.Forms.Padding(0);
            this.throughputDisplay1.Name = "throughputDisplay1";
            this.throughputDisplay1.Size = new System.Drawing.Size(1419, 109);
            this.throughputDisplay1.TabIndex = 10;
            // 
            // SimulatedOS
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(13F, 32F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.throughputDisplay1);
            this.Controls.Add(this.dataTable1);
            this.Controls.Add(this.cpu);
            this.Controls.Add(this.proctable);
            this.Margin = new System.Windows.Forms.Padding(4, 2, 4, 2);
            this.Name = "SimulatedOS";
            this.Size = new System.Drawing.Size(1419, 1297);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private ProcessTable proctable;
        private CPUVisual cpu;
        private DataTable dataTable1;
        private ThroughputDisplay throughputDisplay1;
    }
}
