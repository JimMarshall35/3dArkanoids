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

        private void OnGameConnectionLost(object sender, GameConnectionChangedArgs e)
        {
            throw new NotImplementedException();
        }

        private void OnGameConnectionAquire(object sender, GameBoardDescription e)
        {
            m_gameBoardDescription = e;
            Loaded = true;
        }

        private async void GetBlock()
        {
            m_gameBoardDescription = await m_gameConnectionService.Client.GetBoardStateAsync();
        }
        public ICommand GetBlockCommand { get; private set; }
        private GameBoardDescription m_gameBoardDescription;
        private bool m_getBlockCommandRunning = false;
        private IGameConnectionService m_gameConnectionService;
    }
}
