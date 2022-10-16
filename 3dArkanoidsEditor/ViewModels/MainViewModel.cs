using _3dArkanoidsEditor.Models;
using _3dArkanoidsEditor.Services;
using _3dArkanoidsEditor.utils;
using _3dArkanoidsEditor.ViewModels.interfaces;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;
using static _3dArkanoidsEditor.Models.EditBlockResult;

namespace _3dArkanoidsEditor.ViewModels
{
    internal class MainViewModel : ViewModelBase
    {
        #region Bindable properties



        public ICommand OnSingleTileEditCommand { get; private set; }
        public ICommand GetBlockCommand { get; private set; }
        public ICommand ConnectCommand { get; private set; }
        public ICommand ChangeToolCommand { get; private set; }

        private BoardEditorToolType m_selectedTool = BoardEditorToolType.Erase;
        public BoardEditorToolType SelectedTool
        {
            get
            {
                return m_selectedTool;
            }
            private set
            {
                m_selectedTool = value;
                OnPropertyChange(nameof(SelectedTool));
            }
        }

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
                OnPropertyChange(nameof(IsAllowedToTryToConnect));
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

        private int m_selectedBlockTypeIndex = 0;
        public int SelectedBlockTypeIndex
        {
            get
            {
                return m_selectedBlockTypeIndex;
            }
            set
            {
                m_selectedBlockTypeIndex = value;
                OnPropertyChange(nameof(SelectedBlockTypeIndex));
                OnPropertyChange(nameof(SelectedBlockCode));
            }
        }


        public byte SelectedBlockCode
        {
            get
            {
                if (BlockOptionsViewModels != null)
                {
                    return BlockOptionsViewModels[SelectedBlockTypeIndex].GameBlockType.GameBlockTypeCode;
                }

                else
                {
                    return 1;
                }
            }
        }

        public bool IsAllowedToTryToConnect
        {
            get
            {
                return !m_loaded && !m_tryingToConnect;
            }
        }

        public List<GameBlockType> GameBlockTypes
        {
            get
            {
                var r = BlockOptionsViewModels
                    .Select(x => x.GameBlockType)
                    .ToList();
                //r.Prepend(new GameBlockType(0, 0, 0, 0, 0));
                return r;
            }
        }

        public ObservableCollection<BlockTypeOptionViewModel> BlockOptionsViewModels { get; private set; } = new ObservableCollection<BlockTypeOptionViewModel>();

        public ObservableCollection<SerializablePropertiesNodeViewModel> SerializablePropertiesNodeViewModels { get; private set; } = new ObservableCollection<SerializablePropertiesNodeViewModel>();

        public ITerminalViewModel GameTerminal { get; private set; }


        #endregion

        #region ctor

        ~MainViewModel()
        {
            m_gameConnectionService.GameConnectionAquired -= OnGameConnectionAquire;
            m_gameConnectionService.GameConnectionLost -= OnGameConnectionLost;
        }

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

            ConnectCommand = new RelayCommand(
                    _ => TryConnectToGame(),
                    null
                );

            ChangeToolCommand = new RelayCommand(
                    obj => TryChangeTool((BoardEditorToolType)obj),
                    null
                );
            
            m_gameConnectionService = gameConnectionService;
            m_gameConnectionService.GameConnectionAquired += OnGameConnectionAquire;
            m_gameConnectionService.GameConnectionLost += OnGameConnectionLost;

        }

        #endregion

        #region Game Connection Handlers

        private void OnGameConnectionLost(object sender, GameConnectionLostEventArgs e)
        {
            TryingToConnect = false;
            GameTerminal.WriteLine(e.Message);
            Loaded = false;

        }

        private void OnGameConnectionAquire(object sender, GameConnectionAquiredEventArgs e)
        {
            BlockOptionsViewModels.Clear();
            foreach (var block in e.GameSettings.GameBlockTypes)
            {
                BlockOptionsViewModels.Add(new BlockTypeOptionViewModel(block));
            }
            OnPropertyChange("GameBlockTypes");

            TryingToConnect = false;
            var board = e.GameBoardDescription;
            PlayFieldX = board.Width;
            PlayFieldY = board.Height;
            PlayFieldZ = board.Depth;
            MasterGameBoard = board;


            Loaded = true;
            GameTerminal.WriteLine("Connected to game");
            m_gameConnectionService.Client.GameBoardStream(m_upateStreamCts.Token, newDescription => MasterGameBoard = newDescription);
            m_gameConnectionService.Client.GetFrameworkStacksStream(m_gameFrameworkUpdateStreamCts.Token, (newStacks) =>
            {
                GameTerminal.WriteLine("Game framework change:");
                GameTerminal.WriteLine("Input");
                foreach(var layer in newStacks.InputStack)
                {
                    GameTerminal.WriteLine(layer.ToString());
                }
                GameTerminal.WriteLine("Update");
                foreach (var layer in newStacks.UpdatableStack)
                {
                    GameTerminal.WriteLine(layer.ToString());
                }
                GameTerminal.WriteLine("Drawable");
                foreach (var layer in newStacks.DrawableStack)
                {
                    GameTerminal.WriteLine(layer.ToString());
                }
                GameTerminal.WriteLine("\n\n");

            });
        }


        #endregion

        #region Command Backing Methods

        void TryChangeTool(BoardEditorToolType newToolTypeSelection)
        {
            
            switch (newToolTypeSelection)
            {
                case BoardEditorToolType.Erase:
                    SelectedBlockTypeIndex = -1;
                    SelectedTool = newToolTypeSelection;
                    break;
                case BoardEditorToolType.Draw:
                    SelectedBlockTypeIndex = 1;
                    SelectedTool = newToolTypeSelection;
                    break;
                case BoardEditorToolType.Fill:
                    break;
                case BoardEditorToolType.Select:
                    break;
                default:
                    throw new InvalidOperationException($"invalid tool type {newToolTypeSelection}");
                    break;
            }
        }

        private async void GetBlock()
        {
            MasterGameBoard = await m_gameConnectionService.Client.GetBoardStateAsync();
            
        }

        private async void OnSingleTileEdit(object e)
        {
            SingleTileEdit edit = (SingleTileEdit)e;
            switch (SelectedTool)
            {
                case BoardEditorToolType.Draw:
                case BoardEditorToolType.Erase:
                    var result = await m_gameConnectionService.Client.ChangeBlockAsync(edit);
                    if (result.EditResult != Result.FAILURE_POINT_OUT_OF_BOUNDS && result.EditResult != Result.OTHER_FAILURE)
                    {
                        // had to make SetAt return a new copy and set MasterGameBoardAgain to get the edits to appear on the
                        // WPF canvas.
                        // having SetAt returning void and calling OnPropertyChange(nameof(MasterGameBoard))
                        // didn't work. A weird and suspect solution
                        MasterGameBoard = MasterGameBoard.SetAt(edit.XPos, edit.YPos, edit.ZPos, edit.NewValue);
                    }

                    GameTerminal.WriteLine(Enum.GetName(typeof(Result), result.EditResult) + " " + result.ErrorMessage
                        + ((result.EditResult == Result.BLOCK_AT_SPACE) ? " old block was type: " + result.BlockCode : ""));
                    break;

                case BoardEditorToolType.Fill:
                    var fillResult = await m_gameConnectionService.Client.SetBoardStateAsync(MasterGameBoard);
                    if (fillResult == SetNewBoardStateResult.SUCCESS)
                    {
                        // had to make SetAt return a new copy and set MasterGameBoardAgain to get the edits to appear on the
                        // WPF canvas.
                        // having SetAt returning void and calling OnPropertyChange(nameof(MasterGameBoard))
                        // didn't work. A weird and suspect solution
                        MasterGameBoard = MasterGameBoard.FloodFill(edit.XPos, edit.YPos, edit.ZPos, edit.NewValue);
                    }

                    //GameTerminal.WriteLine(Enum.GetName(typeof(Result), result.EditResult) + " " + result.ErrorMessage
                    //    + ((result.EditResult == Result.BLOCK_AT_SPACE) ? " old block was type: " + result.BlockCode : ""));
                    break;
                case BoardEditorToolType.Select:
                    break;
            }
            
        }

        public void TryConnectToGame()
        {
            m_connectionCts = new CancellationTokenSource();
            m_upateStreamCts = new CancellationTokenSource();
            m_gameFrameworkUpdateStreamCts = new CancellationTokenSource();

            TryingToConnect = true;
            GameTerminal.WriteLine("Trying to connect to game...");
            m_connectionCts.CancelAfter(15000);
            m_gameConnectionService.Connect(m_connectionCts.Token);
            GetSerializableProperties();
            
        }

        #endregion

        #region Helpers
        private async Task GetSerializableProperties()
        {
            var result = await m_gameConnectionService.Client.GetSerializableNodesAsync();
            //SerializablePropertiesNodes.Add(result);//ObservableCollection<SerializablePropertiesNode>(result);
            SerializablePropertiesNodeViewModels.Clear();
            foreach (var r in result)
            {
                SerializablePropertiesNodeViewModels.Add(new SerializablePropertiesNodeViewModel(r,m_gameConnectionService.Client));
            }
        }
        #endregion

        #region private set only properties?!

        private bool m_tryingToConnect = false;
        private bool TryingToConnect
        {
            set
            {
                m_tryingToConnect = value;
                OnPropertyChange(nameof(IsAllowedToTryToConnect));
            }
        }

        #endregion

        private bool m_getBlockCommandRunning = false;
        private IGameConnectionService m_gameConnectionService;
        private CancellationTokenSource m_connectionCts;
        private CancellationTokenSource m_upateStreamCts;
        private CancellationTokenSource m_gameFrameworkUpdateStreamCts;

    }
}
