using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Services
{
    enum SetNewBoardStateResult
    {
        SUCCESS = 0,
        FAILURE_TOO_MANY_BYTES_FOR_WHD = 1,
        FAILURE_TOO_FEW_BYTES_FOR_WHD = 2,
        OTHER_FAILURE = 3
    }
}
