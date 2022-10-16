﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _3dArkanoidsEditor.Models
{
    public class GameFrameworkStackItem
    {
        public GameFrameworkStackItem(string name, bool masksPrevious)
        {
            Name = name;
            MasksPrevious = masksPrevious;
        }

        public string Name { get; private set; }
        public bool MasksPrevious { get; private set; }

        public string ToString()
        {
            return $"Layer {Name}, Masks previous: {MasksPrevious}";
        }

    }
}