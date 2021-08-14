using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using WarlockSimulatorTBC.Shared.Classes.UI;

namespace WarlockSimulatorTBC.Shared.Classes
{
	public static class SpellType
	{
		public static string AFFLICTION = "affliction";
		public static string DESTRUCTION = "destruction";
	}

	public static class SpellSchool
	{
		public static string SHADOW = "shadow";
		public static string FIRE = "fire";
	}

	/*public struct Spell
	{
		Player Player { get; set; }
		int MinDmg { get; set; }
		int MaxDmg { get; set; }
		double Dmg { get; set; }
		double CastTime { get; set; }
		double ManaCost { get; set; }
		double Cooldown { get; set; }
		double Modifier { get; set; }
		double ManaReturn { get; set; }
		double BonusCrit { get; set; }
		double Coefficient { get; set; }
		double CooldownRemaining { get; set; }
		string Name { get; set; }
		string VarName { get; set; } // Same as 'name' except it's written in camelCase
		string Type { get; set; }
		string School { get; set; }
		string BreakdownTable { get; set; }
		bool IsDot { get; set; }
		bool DoesDamage { get; set; }
		bool CanCrit { get; set; }
		public bool Casting { get; set; }
		bool IsItem { get; set; }
		bool OnGcd { get; set; }
		bool IsProc { get; set; }

		public Spell(string name, Player p, int minDmg, int maxDmg, double castTime, double manaCost, double cooldown, double modifier, double coefficient, string type, string school, double manaReturn, double bonusCrit, string breakdownTable, bool isDot, bool doesDamage, bool canCrit, bool isItem, bool onGcd, bool isProc)
		{
			Name = name;
			VarName = name;
			Player = p;
			MinDmg = minDmg;
			MaxDmg = maxDmg;
			Dmg = (minDmg + maxDmg / 2);
			CastTime = castTime;
			Cooldown = cooldown;
			Modifier = modifier;
			ManaCost = manaCost;
			Coefficient = coefficient;
			Type = type;
			School = school;
			ManaReturn = manaReturn;
			BonusCrit = bonusCrit;
			IsDot = isDot;
			DoesDamage = doesDamage;
			BreakdownTable = breakdownTable;
			CanCrit = canCrit;
			IsItem = isItem;
			OnGcd = onGcd;
			IsProc = isProc;
			CooldownRemaining = 0;
			Casting = false;
		}

		public void Reset()
		{
			CooldownRemaining = 0;
			Casting = false;
		}

		public bool CanCast()
		{
			return (!OnGcd || Player.gcdRemaining <= 0) && (IsProc || Player.castTimeRemaining <= 0) && CooldownRemaining <= 0;
		}

		public bool HasEnoughMana()
		{
			//return manaCost <= player.stats.mana;
			return true;
		}

		public bool Ready()
		{
			return CanCast() && HasEnoughMana();
		}

		public void StartCast()
		{
			if (OnGcd)
			{
				Player.gcdRemaining = Player.GetGcdValue();
			}

			if (CastTime > 0)
			{
				Casting = true;
				Player.castTimeRemaining = CastTime;
				Player.CombatLog("Started casting " + Name);
			}
			else
			{
				Cast();
			}
		}

		public void Cast()
		{
			Player.CombatLog("Cast " + Name);
			Player.stats.currentMana -= ManaCost;
			CooldownRemaining = Cooldown;
			Casting = false;

			if (DoesDamage)
			{
				Player.CombatLog("Should do damage with " + Name + " now");
				Damage();
			}
			else
			{
				Player.CombatLog("Doesn't do dmg");
			}
		}

		public void Damage()
		{
			Player.CombatLog("Doing damage with " + Name);
			double dmg = Dmg;
			dmg += (Player.stats.spellPower + School == SpellSchool.SHADOW ? Player.stats.shadowPower : Player.stats.firePower) * Coefficient;
			dmg *= School == SpellSchool.SHADOW ? Player.stats.shadowModifier : School == SpellSchool.FIRE ? Player.stats.fireModifier : 1;

			Player.CombatLog(Name + " " + dmg);
			Player.iterationDamage += dmg;
		}

		public void Tick(double time)
		{
			CooldownRemaining -= time;

			if (Casting && Player.castTimeRemaining <= 0)
			{
				Player.CombatLog("Should cast " + Name + " now");
				Cast();
			}
			else
			{
				if (!Casting)
				{
					Player.CombatLog("Not casting " + Name);
				}
				Player.CombatLog("Player cast time remaining: " + Player.castTimeRemaining);
			}
		}
	}*/

	public class Spell
	{
		protected Player player;
		protected int minDmg = 0;
		protected int maxDmg = 0;
		protected double avgDmg = 0;
		protected double castTime = 0;
		protected double manaCost = 0;
		protected double cooldown = 0;
		protected double modifier = 1;
		public double manaReturn = 0;
		protected double bonusCrit = 0;
		protected double coefficient = 0;
		public double cooldownRemaining = 0;
		public string name = null;
		protected string varName = null; // Same as 'name' except it's written in camelCase
		protected string type = null;
		protected string school = null;
		protected string breakdownTable = "damage";
		protected bool isDot = false;
		protected bool doesDamage = false;
		protected bool canCrit = false;
		public bool casting = false;
		protected bool isItem = false;
		protected bool onGcd = true;
		protected bool isProc = false;

		public Spell(Player p)
		{
			player = p;
		}

		public void Reset()
		{
			cooldownRemaining = 0;
			casting = false;
		}

		public void Setup()
		{

		}

		public bool CanCast()
		{
			return (!onGcd || player.gcdRemaining <= 0) && (isProc || player.castTimeRemaining <= 0) && cooldownRemaining <= 0;
		}

		public bool HasEnoughMana()
		{
			//return manaCost <= player.stats.mana;
			return true;
		}

		public bool Ready()
		{
			return CanCast() && HasEnoughMana();
		}

		public virtual double CalculateCastTime()
		{
			return castTime;
		}

		public void StartCast()
		{
			if (onGcd)
			{
				player.gcdRemaining = player.GetGcdValue();
			}

			if (castTime > 0)
			{
				casting = true;
				player.castTimeRemaining = (double)CalculateCastTime();
			}
			else
			{
				Cast();
			}
		}

		public virtual void Cast()
		{
			player.stats.currentMana -= manaCost;
			cooldownRemaining = (double)cooldown;
			casting = false;

			if (doesDamage)
			{
				player.CombatLog("Doing damage with " + name);
				Damage();
			}
		}

		public void Damage()
		{
			double dmg = (minDmg + maxDmg) / 2;
			dmg += (player.stats.spellPower + (school == SpellSchool.SHADOW ? player.stats.shadowPower : school == SpellSchool.FIRE ? player.stats.firePower : 0)) * coefficient;
			dmg *= school == SpellSchool.SHADOW ? player.stats.shadowModifier : school == SpellSchool.FIRE ? player.stats.fireModifier : 1;
			player.CombatLog("Spell Power: " + player.stats.spellPower);

			player.CombatLog(name + " " + dmg);
			player.iterationDamage += dmg;
		}

		public void Tick(double time)
		{
			cooldownRemaining = Math.Max(0, cooldownRemaining - time);

			if (casting && player.castTimeRemaining <= 0)
			{
				player.CombatLog("Casting " + name);
				Cast();
			}
		}
	}

	public class ShadowBolt : Spell
	{
		public ShadowBolt(Player p) : base(p)
		{
			name = "Shadow Bolt";
			castTime = CalculateCastTime();
			coefficient = 3 / 3.5; // check for Shadow and Flame
			minDmg = 544;
			maxDmg = 607;
			doesDamage = true;
			canCrit = true;
			school = SpellSchool.SHADOW;
			type = SpellType.DESTRUCTION;
			// call Setup() but check whether this constructor runs first or the base constructor. If base then just put Setup() in the base constructor
		}

		public override double CalculateCastTime()
		{
			return 3 - player.talents["bane"] / 10.0 + (double)player.spellDelay;
		}
	}

	public class LifeTap : Spell
	{
		public LifeTap(Player p) : base(p)
		{
			name = "Life Tap";
			coefficient = 0.8;
			manaReturn = 582;
			modifier = 1 * (1 + 0.1 * p.talents["improvedLifeTap"]);
		}

		public double ManaGain()
		{
			return (manaReturn + (player.stats.spellPower + player.stats.shadowPower) * coefficient) * modifier;
		}

		public override void Cast()
		{
			player.stats.currentMana = Math.Min(player.stats.maxMana, player.stats.currentMana + ManaGain());
		}
	}
}
