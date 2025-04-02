using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OSSim.Components
{
    /// <summary>            
    /// Primitive Process Class.
    /// Holds data for each individual process, alongside calculations based off of the data.
    /// </summary>
    public class Process
    {
        // Basic fields.
        public int Arrivaltime { get; set; } = 0;
        public string ProcessID { get; set; } = "";
        // Entire service time the process needs to spend on the cpu
        public int ServiceTime { get; set; } = 0;
        // Holds a copy of the service time for the process that will not be changed
        public int StaticServiceTime { get; set; } = 0;
        // How much more time this process needs on the cpu, should start as service time
        public int TimeRemaining = 0;

        public int Priority { get; set; } = 0;
        // Finish Time starts as -1, if > 0 proc is finished
        public int FinishTime { get; set; } = -1;

        // Calculated properties
        public int TAT
        {
            get
            {
                return FinishTime - Arrivaltime;
            }
        }
        
        public decimal NTAT
        {
            get
            {
                return Convert.ToDecimal(TAT) / ServiceTime;
            }
        }
    }
}
