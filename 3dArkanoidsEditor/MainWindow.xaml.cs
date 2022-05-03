using Grpc.Net.Client;
using System;
using System.Collections.Generic;
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

namespace _3dArkanoidsEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private EditorGRPC.PlayBoardEdit.PlayBoardEditClient m_client;
        private GrpcChannel m_channel;
        public MainWindow()
        {
            InitializeComponent();
            m_channel = GrpcChannel.ForAddress("http://localhost:50051");
            m_client = new EditorGRPC.PlayBoardEdit.PlayBoardEditClient(m_channel);

        }

        private async void Button_Click(object sender, RoutedEventArgs e)
        {
            var result = await m_client.GetBoardStateAsync(new EditorGRPC.Void());
            Console.WriteLine(result.Depth);
        }

        private async void Button_Click_1(object sender, RoutedEventArgs e)
        {
            var result = m_client.AddBlockAsync(new EditorGRPC.Point());
        }
    }
}
