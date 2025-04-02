
namespace OSSim.UI
{
    partial class ProcessTable
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
            this.PnlTable = new System.Windows.Forms.TableLayoutPanel();
            this.LblProcName = new System.Windows.Forms.Label();
            this.ProcTime = new System.Windows.Forms.Label();
            this.PnlTable.SuspendLayout();
            this.SuspendLayout();
            // 
            // PnlTable
            // 
            this.PnlTable.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.PnlTable.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.PnlTable.ColumnCount = 2;
            this.PnlTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.PnlTable.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.PnlTable.Controls.Add(this.LblProcName, 0, 0);
            this.PnlTable.Controls.Add(this.ProcTime, 1, 0);
            this.PnlTable.Dock = System.Windows.Forms.DockStyle.Fill;
            this.PnlTable.Location = new System.Drawing.Point(0, 0);
            this.PnlTable.Name = "PnlTable";
            this.PnlTable.RowCount = 2;
            this.PnlTable.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.PnlTable.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.PnlTable.Size = new System.Drawing.Size(500, 500);
            this.PnlTable.TabIndex = 0;
            // 
            // LblProcName
            // 
            this.LblProcName.AutoSize = true;
            this.LblProcName.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.LblProcName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LblProcName.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            this.LblProcName.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.LblProcName.Location = new System.Drawing.Point(4, 1);
            this.LblProcName.Name = "LblProcName";
            this.LblProcName.Size = new System.Drawing.Size(242, 41);
            this.LblProcName.TabIndex = 0;
            this.LblProcName.Text = "Process Name";
            this.LblProcName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ProcTime
            // 
            this.ProcTime.AutoSize = true;
            this.ProcTime.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.ProcTime.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ProcTime.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            this.ProcTime.Location = new System.Drawing.Point(253, 1);
            this.ProcTime.Name = "ProcTime";
            this.ProcTime.Size = new System.Drawing.Size(243, 41);
            this.ProcTime.TabIndex = 1;
            this.ProcTime.Text = "Service Time";
            this.ProcTime.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // ProcessTable
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(17F, 41F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.PnlTable);
            this.Name = "ProcessTable";
            this.Size = new System.Drawing.Size(500, 500);
            this.PnlTable.ResumeLayout(false);
            this.PnlTable.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        protected System.Windows.Forms.TableLayoutPanel PnlTable;
        private System.Windows.Forms.Label LblProcName;
        private System.Windows.Forms.Label ProcTime;
    }
}
