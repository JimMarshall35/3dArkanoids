using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Models
{
	public class EditBlockResult
	{
        public EditBlockResult(Result result, int blockCode, string errorMessage)
        {

			BlockCode = blockCode;
			ErrorMessage = errorMessage;
			EditResult = result;
        }
		public enum Result
		{
			SPACE_EMPTY = 0,
			BLOCK_AT_SPACE = 1,
			FAILURE_POINT_OUT_OF_BOUNDS = 2,
			OTHER_FAILURE = 3
		}
		public int BlockCode { get; private set; }
		public string ErrorMessage { get; private set; }
		public Result EditResult { get; private set; }
	}
}
