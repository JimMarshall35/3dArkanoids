using _3dArkanoidsEditor.Models;
using _3dArkanoidsEditor.ViewModels;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace _3dArkanoidsEditor.Views
{

    /// <summary>
    /// Interaction logic for PlayBoardCanvas.xaml
    /// </summary>
    public partial class PlayBoardCanvas : UserControl
    {
        public enum CoordsSetFlags : byte
        {
            None = 0,
            X = 1,
            Y = 2,
            Z = 4
        }

        public int PlayBoardTilesX { get; set; }
        public static readonly DependencyProperty PlayBoardTilesXProperty =
            DependencyProperty.Register(nameof(PlayBoardTilesX), typeof(int), typeof(PlayBoardCanvas), 
                new FrameworkPropertyMetadata(OnBoardSizeDependencyPropertyChanged));

        public int PlayBoardTilesY { get; set; }
        public static readonly DependencyProperty PlayBoardTilesYProperty =
            DependencyProperty.Register(nameof(PlayBoardTilesY), typeof(int), typeof(PlayBoardCanvas),
                new FrameworkPropertyMetadata(OnBoardSizeDependencyPropertyChanged));

        public int PlayBoardTilesZ { get; set; }
        public static readonly DependencyProperty PlayBoardTilesZProperty =
        DependencyProperty.Register(nameof(PlayBoardTilesZ), typeof(int), typeof(PlayBoardCanvas),
            new FrameworkPropertyMetadata(OnBoardSizeDependencyPropertyChanged));

        private GameBoardDescription m_playBoardDescription;
        public GameBoardDescription PlayBoardDescription { get; set; }
        public static readonly DependencyProperty PlayBoardDescriptionProperty =
        DependencyProperty.Register(nameof(PlayBoardDescription), typeof(GameBoardDescription), typeof(PlayBoardCanvas),
            new FrameworkPropertyMetadata(OnBoardDescriptionDependencyPropertyChanged));

        public ICommand SingleTileEdit { get; set; }
        public static readonly DependencyProperty SingleTileEditProperty =
        DependencyProperty.Register(nameof(SingleTileEdit), typeof(ICommand), typeof(PlayBoardCanvas),
            new FrameworkPropertyMetadata(OnSingleTileEditDependencyPropertyChanged));

        public byte SelectedBlockCode { get; set; }
        public static readonly DependencyProperty SelectedBlockCodeProperty =
        DependencyProperty.Register(nameof(SelectedBlockCode), typeof(byte), typeof(PlayBoardCanvas),
            new FrameworkPropertyMetadata(OnSelectedBlockCodeDependencyPropertyChanged));

        public List<GameBlockType> GameBlockTypes { get; set; }
        public static readonly DependencyProperty GameBlockTypesProperty =
        DependencyProperty.Register(nameof(GameBlockTypes), typeof(List<GameBlockType>), typeof(PlayBoardCanvas),
            new FrameworkPropertyMetadata(OnGameBlockTypesDependencyPropertyChange));

        

        public PlayBoardCanvas()
        {
            InitializeComponent();
            //DataContext = new PlayBoardCanvasViewModel(this);
            m_canvasZLayerChangerVm = 
                new CanvasZLayerChangerViewModel(OnLayerChange, PlayBoardTilesZ);
            m_zSlider.DataContext = m_canvasZLayerChangerVm;
        }

        private void OnLayerChange(int newLayer)
        {
            m_currentLayer = newLayer;
            UpdateGrid();
        }


        public void RespondToCoordsSet(CoordsSetFlags whichCoordSet)
        {
            m_coordsSetFlags |= whichCoordSet;
            if (m_coordsSetFlags.HasFlag(CoordsSetFlags.X) &&
                m_coordsSetFlags.HasFlag(CoordsSetFlags.Y) &&
                m_coordsSetFlags.HasFlag(CoordsSetFlags.Z))
            {
                m_coordsSetFlags = CoordsSetFlags.None;
                OnAllCoordinatesSet();
            }
        }

        private void UpdateGrid()
        {
            m_playBoardCanvas.Children.Clear();
            for (int x = 0; x < PlayBoardTilesX; x++)
            {
                for (int y = 0; y < PlayBoardTilesY; y++)
                {
                    var byteAtCoords = PlayBoardDescription.GetAt(x, y, m_currentLayer);
                    var colourOfRect = m_byteToColourDict[byteAtCoords];
                    var rectBrush = new SolidColorBrush(colourOfRect);

                    var rect = new Rectangle();
                    rect.Stroke = m_blockOutlineBrush;
                    rect.Fill = rectBrush;
                    rect.Width = m_blockWidthPixels * m_canvasScale;
                    rect.Height = m_blockHeightPixels * m_canvasScale;
                    rect.StrokeThickness = 2;
                    Canvas.SetLeft(rect, x * m_blockWidthPixels * m_canvasScale);
                    Canvas.SetTop(rect, y * m_blockHeightPixels * m_canvasScale);

                    // Possible memory leak ?
                    rect.MouseEnter += (object sender, MouseEventArgs e) =>
                    {
                        rect.Fill = m_blockHoveredFill;
                    };
                    rect.MouseLeave += (object sender, MouseEventArgs e) =>
                    {
                        rect.Fill = rectBrush;
                    };
                    // got to do this to stop the lambda capturing by reference (i think)
                    // https://stackoverflow.com/questions/451779/how-to-tell-a-lambda-function-to-capture-a-copy-instead-of-a-reference-in-c
                    int tileX = x;
                    int tileY = y;
                    rect.MouseLeftButtonDown += (object sender, MouseButtonEventArgs e) =>
                    {
                        SingleTileEdit.Execute(new SingleTileEdit(tileX, tileY, m_currentLayer, SelectedBlockCode, byteAtCoords));

                    };
                    m_playBoardCanvas.Children.Add(rect);

                }
            }
            m_canvasZLayerChangerVm.Visibility = Visibility.Visible;
        }

        private void OnAllCoordinatesSet()
        {
            m_playBoardCanvas.Width = PlayBoardTilesX * m_blockWidthPixels * m_canvasScale;
            m_playBoardCanvas.Height = PlayBoardTilesY * m_blockHeightPixels * m_canvasScale;
            m_canvasZLayerChangerVm.SetMaxLayerIndex(PlayBoardTilesZ);
            //UpdateGrid();
        }

        private static void OnBoardSizeDependencyPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var canvas = ((PlayBoardCanvas)d);
            switch (e.Property.Name)
            {
                case nameof(PlayBoardTilesX):
                    canvas.PlayBoardTilesX = (int)e.NewValue;
                    canvas.RespondToCoordsSet(CoordsSetFlags.X);
                    break;
                case nameof(PlayBoardTilesY):
                    canvas.PlayBoardTilesY = (int)e.NewValue;
                    canvas.RespondToCoordsSet(CoordsSetFlags.Y);
                    break;
                case nameof(PlayBoardTilesZ):
                    canvas.PlayBoardTilesZ = (int)e.NewValue;
                    canvas.RespondToCoordsSet(CoordsSetFlags.Z);
                    break;
                default:
                    Console.WriteLine("jiodfijddddd");
                    break;
            }
        }

        private static void OnBoardDescriptionDependencyPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var newVal = e.NewValue;
            ((PlayBoardCanvas)d).PlayBoardDescription = (GameBoardDescription)e.NewValue;
            ((PlayBoardCanvas)d).UpdateGrid();

        }

        private static void OnSelectedBlockCodeDependencyPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var canvas = ((PlayBoardCanvas)d);
            canvas.SelectedBlockCode = (byte)e.NewValue;
        }

        private static void OnSingleTileEditDependencyPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var newVal = e.NewValue;
            ((PlayBoardCanvas)d).SingleTileEdit = (ICommand)e.NewValue;
        }

        private static void OnGameBlockTypesDependencyPropertyChange(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var canvas = ((PlayBoardCanvas)d);
            var list = (List<GameBlockType>)e.NewValue;
            if (list.Count == 0)
                return;
            canvas.m_byteToColourDict = list.ToDictionary(x => x.GameBlockTypeCode, x => x.Colour.ToWpfColor());
            canvas.m_byteToColourDict.Add(0x00, m_blockNotHoveredFillColour);
            canvas.UpdateGrid();
        }


        private readonly SolidColorBrush m_blockOutlineBrush = new SolidColorBrush(Colors.Black);
        private readonly SolidColorBrush m_blockHoveredFill = new SolidColorBrush(Colors.BlanchedAlmond);
        private static readonly Color m_blockNotHoveredFillColour = Colors.Transparent;
        private Dictionary<byte, Color> m_byteToColourDict = new Dictionary<byte, Color>();

        private double m_canvasScale = 2.0;
        private double m_blockWidthPixels = 16.0f;
        private double m_blockHeightPixels = 8.0f;
        private CoordsSetFlags m_coordsSetFlags = CoordsSetFlags.None;
        private int m_currentLayer = 0;
        private CanvasZLayerChangerViewModel m_canvasZLayerChangerVm;
    }
}
