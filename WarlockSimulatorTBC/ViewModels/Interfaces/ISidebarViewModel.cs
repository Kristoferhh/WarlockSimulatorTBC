using System.Threading.Tasks;

namespace WarlockSimulatorTBC.ViewModels.Interfaces
{
    public interface ISidebarViewModel
    {
        string AvgDps { get; set; }
        string MaxDps { get; set; }
        string MinDps { get; set; }
        string MultiSimButtonText { get; set; }
        uint MultiSimProgress { get; set; }
        uint SimProgress { get; set; }
        string SimulateButtonText { get; set; }
        string SimulationDuration { get; set; }

        Task InitializeViewModel();
        void OnWorkerMessage(object sender, string message);
        void RefreshCharacterStats();
        void SimulateDps(string simulationType);
    }
}