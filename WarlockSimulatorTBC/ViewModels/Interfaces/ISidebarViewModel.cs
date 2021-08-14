using System.ComponentModel;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.ViewModels.Interfaces
{
    public interface ISidebarViewModel : IBaseViewModel
    {
        string AvgDps { get; set; }
        string MaxDps { get; set; }
        string MinDps { get; set; }
        string MultiSimButtonText { get; set; }
        int MultiSimProgress { get; set; }
        int SimProgress { get; set; }
        string SimulateButtonText { get; set; }
        string SimulationDuration { get; set; }

        Task InitializeViewModel();
        void OnWorkerMessage(object sender, string message);
        void SimulateDps(string simulationType);
    }
}