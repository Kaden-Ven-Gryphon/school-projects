
namespace OSSim
{
    partial class MainWindow
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel1 = new System.Windows.Forms.Panel();
            this.RRTextBox = new System.Windows.Forms.TextBox();
            this.RRLabel = new System.Windows.Forms.Label();
            this.Sim2 = new OSSim.UI.SimulatedOS();
            this.Sim1 = new OSSim.UI.SimulatedOS();
            this.LblElapsed = new System.Windows.Forms.Label();
            this.lblInterval = new System.Windows.Forms.Label();
            this.txtInterval = new System.Windows.Forms.TextBox();
            this.LblSimulationStatus = new System.Windows.Forms.Label();
            this.BtnPause = new System.Windows.Forms.Button();
            this.BtnStart = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.RRTextBox);
            this.panel1.Controls.Add(this.RRLabel);
            this.panel1.Controls.Add(this.Sim2);
            this.panel1.Controls.Add(this.Sim1);
            this.panel1.Controls.Add(this.LblElapsed);
            this.panel1.Controls.Add(this.lblInterval);
            this.panel1.Controls.Add(this.txtInterval);
            this.panel1.Controls.Add(this.LblSimulationStatus);
            this.panel1.Controls.Add(this.BtnPause);
            this.panel1.Controls.Add(this.BtnStart);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Margin = new System.Windows.Forms.Padding(1, 1, 1, 1);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1527, 738);
            this.panel1.TabIndex = 0;
            // 
            // RRTextBox
            // 
            this.RRTextBox.Location = new System.Drawing.Point(1402, 126);
            this.RRTextBox.Name = "RRTextBox";
            this.RRTextBox.Size = new System.Drawing.Size(69, 23);
            this.RRTextBox.TabIndex = 5;
            this.RRTextBox.Text = "5";
            this.RRTextBox.TextChanged += new System.EventHandler(this.RRTextBox_TextChanged);
            // 
            // RRLabel
            // 
            this.RRLabel.AutoSize = true;
            this.RRLabel.Location = new System.Drawing.Point(1208, 126);
            this.RRLabel.Name = "RRLabel";
            this.RRLabel.Size = new System.Drawing.Size(172, 15);
            this.RRLabel.TabIndex = 1;
            this.RRLabel.Text = "Round Robin Time Slice Length";
            // 
            // Sim2
            // 
            this.Sim2.Location = new System.Drawing.Point(760, 115);
            this.Sim2.Margin = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.Sim2.Name = "Sim2";
            this.Sim2.Size = new System.Drawing.Size(767, 613);
            this.Sim2.TabIndex = 11;
            // 
            // Sim1
            // 
            this.Sim1.Location = new System.Drawing.Point(0, 115);
            this.Sim1.Margin = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.Sim1.Name = "Sim1";
            this.Sim1.Size = new System.Drawing.Size(757, 613);
            this.Sim1.TabIndex = 10;
            // 
            // LblElapsed
            // 
            this.LblElapsed.AutoSize = true;
            this.LblElapsed.Location = new System.Drawing.Point(678, 47);
            this.LblElapsed.Margin = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.LblElapsed.Name = "LblElapsed";
            this.LblElapsed.Size = new System.Drawing.Size(79, 15);
            this.LblElapsed.TabIndex = 9;
            this.LblElapsed.Text = "Elapsed Time:";
            // 
            // lblInterval
            // 
            this.lblInterval.AutoSize = true;
            this.lblInterval.Location = new System.Drawing.Point(249, 47);
            this.lblInterval.Margin = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.lblInterval.Name = "lblInterval";
            this.lblInterval.Size = new System.Drawing.Size(85, 15);
            this.lblInterval.TabIndex = 6;
            this.lblInterval.Text = "Time Unit (ms)";
            // 
            // txtInterval
            // 
            this.txtInterval.Location = new System.Drawing.Point(345, 45);
            this.txtInterval.Margin = new System.Windows.Forms.Padding(1, 1, 1, 1);
            this.txtInterval.Name = "txtInterval";
            this.txtInterval.Size = new System.Drawing.Size(69, 23);
            this.txtInterval.TabIndex = 5;
            this.txtInterval.Text = "500";
            this.txtInterval.TextChanged += new System.EventHandler(this.TxtInterval_TextChanged);
            // 
            // LblSimulationStatus
            // 
            this.LblSimulationStatus.AutoSize = true;
            this.LblSimulationStatus.Location = new System.Drawing.Point(585, 5);
            this.LblSimulationStatus.Margin = new System.Windows.Forms.Padding(1, 0, 1, 0);
            this.LblSimulationStatus.Name = "LblSimulationStatus";
            this.LblSimulationStatus.Size = new System.Drawing.Size(86, 15);
            this.LblSimulationStatus.TabIndex = 4;
            this.LblSimulationStatus.Text = "System Paused";
            // 
            // BtnPause
            // 
            this.BtnPause.Location = new System.Drawing.Point(487, 5);
            this.BtnPause.Margin = new System.Windows.Forms.Padding(1, 1, 1, 1);
            this.BtnPause.Name = "BtnPause";
            this.BtnPause.Size = new System.Drawing.Size(67, 24);
            this.BtnPause.TabIndex = 3;
            this.BtnPause.Text = "Pause";
            this.BtnPause.UseVisualStyleBackColor = true;
            this.BtnPause.Click += new System.EventHandler(this.BtnPause_Click);
            // 
            // BtnStart
            // 
            this.BtnStart.Location = new System.Drawing.Point(354, 5);
            this.BtnStart.Margin = new System.Windows.Forms.Padding(1, 1, 1, 1);
            this.BtnStart.Name = "BtnStart";
            this.BtnStart.Size = new System.Drawing.Size(67, 24);
            this.BtnStart.TabIndex = 2;
            this.BtnStart.Text = "Start";
            this.BtnStart.UseVisualStyleBackColor = true;
            this.BtnStart.Click += new System.EventHandler(this.BtnStart_Click);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1527, 738);
            this.Controls.Add(this.panel1);
            this.Margin = new System.Windows.Forms.Padding(1, 1, 1, 1);
            this.Name = "MainWindow";
            this.Text = "OSSim";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button BtnStart;
        private System.Windows.Forms.Button BtnPause;
        private System.Windows.Forms.Label LblSimulationStatus;
        private System.Windows.Forms.Label lblInterval;
        private System.Windows.Forms.TextBox txtInterval;
        private System.Windows.Forms.Label LblElapsed;
        private UI.SimulatedOS Sim1;
        private UI.SimulatedOS Sim2;
        private System.Windows.Forms.TextBox RRTextBox;
        private System.Windows.Forms.Label RRLabel;
    }
}

