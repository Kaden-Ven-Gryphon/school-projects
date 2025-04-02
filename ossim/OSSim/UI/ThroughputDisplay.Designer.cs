
namespace OSSim.UI
{
    partial class ThroughputDisplay
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
            this.LblThroughput = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // LblThroughput
            // 
            this.LblThroughput.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LblThroughput.Location = new System.Drawing.Point(0, 0);
            this.LblThroughput.Name = "LblThroughput";
            this.LblThroughput.Size = new System.Drawing.Size(1063, 289);
            this.LblThroughput.TabIndex = 0;
            this.LblThroughput.Text = "Current Throughput:";
            // 
            // ThroughputDisplay
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(17F, 41F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.LblThroughput);
            this.Name = "ThroughputDisplay";
            this.Size = new System.Drawing.Size(1063, 289);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label LblThroughput;
    }
}
