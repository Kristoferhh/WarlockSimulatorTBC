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

	public class Spell
	{
		protected Player player;
		protected int minDmg = 0;
		protected int maxDmg = 0;
		protected double dmg = 0;
		protected double castTime = 0;
		protected double manaCost = 0;
		protected double cooldown = 0;
		protected double modifier = 1;
		public double manaReturn = 0;
		protected double bonusCrit = 0;
		protected double coefficient = 0;
		public decimal cooldownRemaining = decimal.Zero;
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
			return (!onGcd || player.gcdRemaining.Equals(decimal.Zero)) && (isProc || player.castTimeRemaining.Equals(decimal.Zero)) && cooldownRemaining.Equals(decimal.Zero);
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
				player.castTimeRemaining = (decimal)CalculateCastTime();
			}
			else
			{
				Cast();
			}
		}

		public virtual void Cast()
		{
			player.stats.currentMana -= manaCost;
			cooldownRemaining = (decimal)cooldown;
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
			dmg += (player.stats.spellPower + school == SpellSchool.SHADOW ? player.stats.shadowPower : player.stats.firePower) * coefficient;
			dmg *= school == SpellSchool.SHADOW ? player.stats.shadowModifier : school == SpellSchool.FIRE ? player.stats.fireModifier : 1;

			player.CombatLog(name + " " + dmg);
			player.iterationDamage += dmg;
		}

		public void Tick(decimal time)
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
			Console.WriteLine(castTime);
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
			return 3 - TalentTree.CurrentTalents["bane"] / 10.0 + (double)player.spellDelay;
		}
	}

	public class LifeTap : Spell
	{
		public LifeTap(Player p) : base(p)
		{
			name = "Life Tap";
			coefficient = 0.8;
			manaReturn = 582;
			Console.WriteLine("here");
			foreach (var talent in p.talents)
			{
				Console.WriteLine(talent.Key + " - " + talent.Value);
			}
			modifier = 1 * (1 + 0.1 * p.talents["improvedLifeTap"]);
			Console.WriteLine("here2");
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
