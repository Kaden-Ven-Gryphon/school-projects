using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using OSSim;
using System.Threading;

namespace TestOSSim.Components
{
    // Unit tests for CPU class
    [TestClass]
    public class CPU
    {
        [TestMethod]
        public void TestTimer()
        {
            // Arrange
            int proctime = 50; // 50 iterations
            OSSim.Components.CPU cpu = new(null);
            OSSim.Components.CPU.TimeUnit = 1; // sleep 2 ms per iteration
            OSSim.Components.Process process1 = new(){ Arrivaltime = 0, ProcessID = "process 1", ServiceTime = 10, Priority = 1 };
            cpu.CurrentProcess = process1;
            OSSim.Components.CPU.Paused = false;

            var wait = Convert.ToInt32(OSSim.Components.CPU.TimeUnit * proctime);
            // Act
            cpu.RunProcess(process1);
            // Sleep for the amount of time allotted to the process sleeping plus an additional 100 ms for each step call
            Thread.Sleep(wait+100);
            // Assert
            Assert.AreEqual(OSSim.Components.CPU.Paused,false);
            Assert.AreEqual("idle",cpu.CurrentStatus.Exec);
            Assert.AreEqual("n/a",cpu.CurrentStatus.TicksLeft );

        }
    }
}
