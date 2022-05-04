using _3dArkanoidsEditor.Models;
using _3dArkanoidsEditor.Services;
using _3dArkanoidsEditor.utils;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace _3dArkanoidsEditor.ViewModels
{
    internal class MainViewModel : ViewModelBase
    {
        #region Bindable properties

        public ICommand GetBlockCommand { get; private set; }

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

        #endregion

        #region ctor

        public MainViewModel(IGameConnectionService gameConnectionService)
        {
            GetBlockCommand = new RelayCommand(
                    _ => GetBlock(), 
                    null
                );
            m_gameConnectionService = gameConnectionService;
            m_gameConnectionService.GameConnectionAquired += OnGameConnectionAquire;
            m_gameConnectionService.GameConnectionLost += OnGameConnectionLost;
            Task.Run(() => m_gameConnectionService.Connect());

        }

        #endregion

        #region Game Connection Handlers

        private void OnGameConnectionLost(object sender, GameConnectionChangedArgs e)
        {
            throw new NotImplementedException();
        }

        private void OnGameConnectionAquire(object sender, GameBoardDescription e)
        {
            m_gameBoardDescription = e;
            PlayFieldX = e.Width;
            PlayFieldY = e.Height;
            PlayFieldZ = e.Depth;
            Loaded = true;
        }

        #endregion

        #region Command Backing Methods

        private async void GetBlock()
        {
            m_gameBoardDescription = await m_gameConnectionService.Client.GetBoardStateAsync();
        }

        #endregion

        private GameBoardDescription m_gameBoardDescription;
        private bool m_getBlockCommandRunning = false;
        private IGameConnectionService m_gameConnectionService;
    }
}
