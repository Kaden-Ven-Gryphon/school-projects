
namespace OSSim.UI
{
    partial class DataTable
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
            this.dataTablePanelRows = new System.Windows.Forms.TableLayoutPanel();
            this.HeaderPanel = new System.Windows.Forms.TableLayoutPanel();
            this.LblProcName = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.dataTablePanelRows.SuspendLayout();
            this.HeaderPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // dataTablePanelRows
            // 
            this.dataTablePanelRows.AutoScroll = true;
            this.dataTablePanelRows.ColumnCount = 1;
            this.dataTablePanelRows.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.dataTablePanelRows.Controls.Add(this.HeaderPanel, 0, 0);
            this.dataTablePanelRows.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataTablePanelRows.Location = new System.Drawing.Point(0, 0);
            this.dataTablePanelRows.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.dataTablePanelRows.Name = "dataTablePanelRows";
            this.dataTablePanelRows.RowCount = 2;
            this.dataTablePanelRows.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.dataTablePanelRows.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.dataTablePanelRows.Size = new System.Drawing.Size(675, 284);
            this.dataTablePanelRows.TabIndex = 6;
            // 
            // HeaderPanel
            // 
            this.HeaderPanel.ColumnCount = 6;
            this.HeaderPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.67706F));
            this.HeaderPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.67706F));
            this.HeaderPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.83292F));
            this.HeaderPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.45886F));
            this.HeaderPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.67706F));
            this.HeaderPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 16.67706F));
            this.HeaderPanel.Controls.Add(this.LblProcName, 0, 0);
            this.HeaderPanel.Controls.Add(this.label8, 1, 0);
            this.HeaderPanel.Controls.Add(this.label9, 2, 0);
            this.HeaderPanel.Controls.Add(this.label10, 3, 0);
            this.HeaderPanel.Controls.Add(this.label11, 4, 0);
            this.HeaderPanel.Controls.Add(this.label12, 5, 0);
            this.HeaderPanel.Location = new System.Drawing.Point(3, 4);
            this.HeaderPanel.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.HeaderPanel.Name = "HeaderPanel";
            this.HeaderPanel.RowCount = 1;
            this.HeaderPanel.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.HeaderPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 161F));
            this.HeaderPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 161F));
            this.HeaderPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 161F));
            this.HeaderPanel.Size = new System.Drawing.Size(667, 47);
            this.HeaderPanel.TabIndex = 9;
            // 
            // LblProcName
            // 
            this.LblProcName.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.LblProcName.Location = new System.Drawing.Point(3, 8);
            this.LblProcName.Name = "LblProcName";
            this.LblProcName.Size = new System.Drawing.Size(104, 31);
            this.LblProcName.TabIndex = 0;
            this.LblProcName.Text = "Process Name";
            this.LblProcName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label8
            // 
            this.label8.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label8.Location = new System.Drawing.Point(114, 8);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(104, 31);
            this.label8.TabIndex = 1;
            this.label8.Text = "Arrival Time";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label9
            // 
            this.label9.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label9.Location = new System.Drawing.Point(225, 8);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(105, 31);
            this.label9.TabIndex = 2;
            this.label9.Text = "Service Time";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label10
            // 
            this.label10.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label10.Location = new System.Drawing.Point(337, 8);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(103, 31);
            this.label10.TabIndex = 3;
            this.label10.Text = "Finish Time";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label11
            // 
            this.label11.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label11.Location = new System.Drawing.Point(446, 8);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(104, 31);
            this.label11.TabIndex = 4;
            this.label11.Text = "TAT";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label12
            // 
            this.label12.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label12.Location = new System.Drawing.Point(557, 8);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(106, 31);
            this.label12.TabIndex = 5;
            this.label12.Text = "nTAT";
            this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // DataTable
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.dataTablePanelRows);
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "DataTable";
            this.Size = new System.Drawing.Size(675, 284);
            this.dataTablePanelRows.ResumeLayout(false);
            this.HeaderPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.TableLayoutPanel dataTablePanelRows;
        private System.Windows.Forms.TableLayoutPanel HeaderPanel;
        private System.Windows.Forms.Label LblProcName;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
    }
}
