using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.VisualStudio.TestTools.UnitTesting;
namespace TestOSSim.UI
{
    // Unit tests for process table gui element
    [TestClass]
    public class ProcessTable
    {
        [TestMethod]
        public void TestProcessTable_Init()
        {
            MockProcessTable p = new();
            Assert.IsNotNull(p.Procs);
        }

        [TestMethod]
        public void TestAddProc(){ 
            // Arrange
            MockProcessTable p = new();
            // Act
            p.AddProc(p.Panel,"Test Object", 400);
            var result = p.Procs;
            var proc = result.ToList().Find((proc) => { return proc.Name.Text == "Test Object";});
            //Assert
            Assert.IsNotNull(proc.TimeCost);
            Assert.AreEqual(proc.TimeCost.Text, "400");
            Assert.AreEqual(4, p.Panel.Controls.Count);


        }

        [TestMethod]
        public void TestRemoveProc()
        {
             // Arrange
            MockProcessTable p = new();
            Assert.AreEqual(0, p.Procs.Count()); // Precondition, list should be empty.
            // Act
            p.AddProc(p.Panel,"Test Object", 400);
            p.RemoveProc(p.Panel,"Test Object");
            var result = p.Procs;
            var proc = result.ToList().Find((proc) => { return proc.Name.Text == "Test Object";});
            //Assert
            Assert.IsNull(proc.TimeCost);
            Assert.IsNull(proc.Name);
            Assert.AreEqual(2, p.Panel.Controls.Count);

        }
    }
}
