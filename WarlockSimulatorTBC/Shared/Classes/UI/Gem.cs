using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace WarlockSimulatorTBC.Shared.Classes.UI
{
	public class SocketInfo
	{
		public string iconName;
		public string[] gems;
	}

	public class Gem
	{
		public int phase;
		public int spellPower;
		public string name;
		public string iconName;
	}

	public static class Sockets
	{
		public static readonly Dictionary<string, SocketInfo> sockets = new Dictionary<string, SocketInfo>
		{
			{
				SocketColor.META, new SocketInfo
				{
					iconName = "UI-EmptySocket-Meta.jpg",
					gems = new string[] {"meta"}
				}
			},
			{
				SocketColor.RED, new SocketInfo
				{
					iconName = "UI-EmptySocket-Red.jpg",
					gems = new string[] {"red", "orange", "purple"}
				}
			},
			{
				SocketColor.YELLOW, new SocketInfo
				{
					iconName = "UI-EmptySocket-Yellow.jpg",
					gems = new string[] {"yellow", "orange", "green"}
				}
			},
			{
				SocketColor.BLUE, new SocketInfo
				{
					iconName = "UI-EmptySocket-Blue.jpg",
					gems = new string[] {"blue", "green", "purple"}
				}
			}
		};
	}

	public static class SocketColor
	{
		public static string META = "meta";
		public static string RED = "red";
		public static string BLUE = "blue";
		public static string YELLOW = "yellow";
	}

	public static class GemColor
	{
		public static string META = "meta";
		public static string RED = "red";
		public static string BLUE = "blue";
		public static string YELLOW = "yellow";
		public static string ORANGE = "orange";
		public static string PURPLE = "purple";
		public static string GREEN = "green";
	}

	public static class Gems
	{
		public static readonly Dictionary<string, Dictionary<int, Gem>> gems = new Dictionary<string, Dictionary<int, Gem>>
		{
			{
				GemColor.RED, new Dictionary<int, Gem>
				{
					{
						24030, new Gem
						{
							name = "Runed Living Ruby",
							iconName = "inv_jewelcrafting_livingruby_03.jpg",
							phase = 1,
							spellPower = 9
						}
					}
				}
			}
		};
	}
}
