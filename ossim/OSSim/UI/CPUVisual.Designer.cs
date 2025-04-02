
namespace OSSim.UI
{
    partial class CPUVisual
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.TimeRemainingLabel = new System.Windows.Forms.Label();
            this.ExecLabel = new System.Windows.Forms.Label();
            this.CPUNumberLabel = new System.Windows.Forms.Label();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.panel1.Controls.Add(this.TimeRemainingLabel);
            this.panel1.Controls.Add(this.ExecLabel);
            this.panel1.Controls.Add(this.CPUNumberLabel);
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(230, 200);
            this.panel1.TabIndex = 0;
            // 
            // TimeRemainingLabel
            // 
            this.TimeRemainingLabel.AutoSize = true;
            this.TimeRemainingLabel.Location = new System.Drawing.Point(16, 83);
            this.TimeRemainingLabel.Name = "TimeRemainingLabel";
            this.TimeRemainingLabel.Size = new System.Drawing.Size(120, 20);
            this.TimeRemainingLabel.TabIndex = 2;
            this.TimeRemainingLabel.Text = "Time Remaining:";
            // 
            // ExecLabel
            // 
            this.ExecLabel.AutoSize = true;
            this.ExecLabel.Location = new System.Drawing.Point(16, 63);
            this.ExecLabel.Name = "ExecLabel";
            this.ExecLabel.Size = new System.Drawing.Size(42, 20);
            this.ExecLabel.TabIndex = 1;
            this.ExecLabel.Text = "Exec:";
            // 
            // CPUNumberLabel
            // 
            this.CPUNumberLabel.AutoSize = true;
            this.CPUNumberLabel.Location = new System.Drawing.Point(15, 43);
            this.CPUNumberLabel.Name = "CPUNumberLabel";
            this.CPUNumberLabel.Size = new System.Drawing.Size(40, 20);
            this.CPUNumberLabel.TabIndex = 0;
            this.CPUNumberLabel.Text = "CPU ";
            // 
            // CPUVisual
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.Controls.Add(this.panel1);
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "CPUVisual";
            this.Size = new System.Drawing.Size(230, 200);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label CPUNumberLabel;
        private System.Windows.Forms.Label TimeRemainingLabel;
        private System.Windows.Forms.Label ExecLabel;
    }
}
