﻿using _3dArkanoidsEditor.Models;
using _3dArkanoidsEditor.Services;
using _3dArkanoidsEditor.utils;
using _3dArkanoidsEditor.ViewModels.interfaces;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using static _3dArkanoidsEditor.Models.EditBlockResult;

namespace _3dArkanoidsEditor.ViewModels
{
    internal class MainViewModel : ViewModelBase
    {
        #region Bindable properties

        public ICommand OnSingleTileEditCommand { get; set; }
        public ICommand GetBlockCommand { get; private set; }

        private GameBoardDescription m_gameBoardDescription;
        public GameBoardDescription MasterGameBoard
        {
            get
            {
                return m_gameBoardDescription;
            }
            set
            {
                m_gameBoardDescription = value;
                OnPropertyChange(nameof(MasterGameBoard));
            }
        }

        private bool m_loaded = false;
        public bool Loaded
        {
            get
            {
                return m_loaded;
            }
            set
            {
                m_loaded = value;
                OnPropertyChange(nameof(Loaded));
            }
        }

        private int m_playFieldX;
        public int PlayFieldX
        {
            get
            {
                return m_playFieldX;
            }
            set
            {
                m_playFieldX = value;
                OnPropertyChange(nameof(PlayFieldX));
            }
        }

        private int m_playFieldY;
        public int PlayFieldY
        {
            get
            {
                return m_playFieldY;
            }
            set
            {
                m_playFieldY = value;
                OnPropertyChange(nameof(PlayFieldY));
            }
        }

        private int m_playFieldZ;
        public int PlayFieldZ
        {
            get
            {
                return m_playFieldZ;
            }
            set
            {
                m_playFieldZ = value;
                OnPropertyChange(nameof(PlayFieldZ));
            }
        }

        public ITerminalViewModel GameTerminal { get; private set; }


        #endregion

        #region ctor

        public MainViewModel(IGameConnectionService gameConnectionService, ITerminalViewModel terminal)
        {
            GameTerminal = terminal;
            GetBlockCommand = new RelayCommand(
                    _ => GetBlock(), 
                    null
                );

            OnSingleTileEditCommand = new RelayCommand(
                    OnSingleTileEdit,
                    null
                );


            m_gameConnectionService = gameConnectionService;
            m_gameConnectionService.GameConnectionAquired += OnGameConnectionAquire;
            m_gameConnectionService.GameConnectionLost += OnGameConnectionLost;
            m_gameConnectionService.Connect().ContinueWith(_ => {
                GameTerminal.WriteLine("Game connected");
            });

        }

        #endregion

        #region Game Connection Handlers

        private void OnGameConnectionLost(object sender, GameConnectionChangedArgs e)
        {
            throw new NotImplementedException();
        }

        private void OnGameConnectionAquire(object sender, GameBoardDescription e)
        {
            PlayFieldX = e.Width;
            PlayFieldY = e.Height;
            PlayFieldZ = e.Depth;
            MasterGameBoard = e;
            Loaded = true;
        }

        #endregion

        #region Command Backing Methods

        private async void GetBlock()
        {
            m_gameBoardDescription = await m_gameConnectionService.Client.GetBoardStateAsync();
            
        }

        private async void OnSingleTileEdit(object e)
        {
            SingleTileEdit edit = (SingleTileEdit)e;
            var result = await m_gameConnectionService.Client.ChangeBlockAsync(edit);
            if(result.EditResult != Result.FAILURE_POINT_OUT_OF_BOUNDS && result.EditResult != Result.OTHER_FAILURE)
            {
                // had to make SetAt return a new copy and set MasterGameBoardAgain to get the edits to appear on the
                // WPF canvas.
                // having SetAt returning void and calling OnPropertyChange(nameof(MasterGameBoard))
                // didn't work. A weird and suspect solution
                MasterGameBoard = MasterGameBoard.SetAt(edit.XPos, edit.YPos, edit.ZPos, edit.NewValue);
            }
            
            GameTerminal.WriteLine(Enum.GetName(typeof(Result), result.EditResult) + " "+result.ErrorMessage
                + ((result.EditResult == Result.BLOCK_AT_SPACE) ? " old block was type: "+result.BlockCode : ""));
        }

        #endregion

        private bool m_getBlockCommandRunning = false;
        private IGameConnectionService m_gameConnectionService;
    }
}
