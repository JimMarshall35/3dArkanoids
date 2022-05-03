﻿using _3dArkanoidsEditor.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Services
{
    interface IGameEditorClient
    {
        Task<GameBoardDescription> GetBoardStateAsync();

    }
}
