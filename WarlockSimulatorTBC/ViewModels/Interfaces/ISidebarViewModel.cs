using Microsoft.AspNetCore.Components;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.ViewModels.Interfaces
{
    public interface ISidebarViewModel : IBaseViewModel
    {
        double CritPercent { get; set; }
        double FireModifier { get; set; }
        double FirePower { get; set; }
        double Health { get; set; }
        double HitPercent { get; set; }
        double Intellect { get; set; }
        double Mana { get; set; }
        string MaxDps { get; set; }
        string MedianDps { get; set; }
        string MinDps { get; set; }
        double MP5 { get; set; }
        string MultiSimButtonText { get; set; }
        int MultiSimProgress { get; set; }
        double ShadowModifier { get; set; }
        double ShadowPower { get; set; }
        int SimProgress { get; set; }
        string SimulateButtonText { get; set; }
        string SimulationDuration { get; set; }
        double SpellCritRating { get; set; }
        double SpellHasteRating { get; set; }
        double SpellHitRating { get; set; }
        double SpellPower { get; set; }
        double Spirit { get; set; }
        double Stamina { get; set; }

        Task InitializeViewModel(EventCallback refreshItemTable);
        void OnWorkerMessage(object sender, string message);
        void RefreshStats();
        Task SimulateDps(string simulationType);
    }
}