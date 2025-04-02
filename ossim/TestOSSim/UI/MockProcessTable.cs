using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OSSim;
namespace TestOSSim.UI
{
    // Mock features used to support process tables.
    // Set variables to protected to be able to expose them here.
    class MockProcessTable : OSSim.UI.ProcessTable
    {
        public IEnumerable<Proc> Procs{ get { return Processes; } }

        public System.Windows.Forms.TableLayoutPanel Panel { get { return PnlTable;  } }
    }
}
