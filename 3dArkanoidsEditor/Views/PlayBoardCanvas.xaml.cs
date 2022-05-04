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


        public PlayBoardCanvas()
        {
            InitializeComponent();
            //DataContext = new PlayBoardCanvasViewModel(this);
        }



        public void RespondToCoordsSet(CoordsSetFlags whichCoordSet)
        {
            m_coordsSetFlags |= whichCoordSet;
            if (m_coordsSetFlags.HasFlag(CoordsSetFlags.X) &&
                m_coordsSetFlags.HasFlag(CoordsSetFlags.Y) &&
                m_coordsSetFlags.HasFlag(CoordsSetFlags.Z))
            {
                m_coordsSetFlags = CoordsSetFlags.None;
                RespondToAllCoordinatesSet();
            }
        }

        private void RespondToAllCoordinatesSet()
        {
            m_playBoardCanvas.Width = PlayBoardTilesX * m_blockWidthPixels;
            m_playBoardCanvas.Height = PlayBoardTilesY * m_blockHeightPixels;
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

        private double m_blockWidthPixels = 16.0f;
        private double m_blockHeightPixels = 8.0f;
        private PlayBoardCanvas m_canvas;
        private CoordsSetFlags m_coordsSetFlags = CoordsSetFlags.None;
    }
}
