using System.ComponentModel;

namespace WarlockSimulatorTBC.ViewModels.Interfaces
{
    public interface IBaseViewModel
    {
        event PropertyChangedEventHandler PropertyChanged;
    }
}