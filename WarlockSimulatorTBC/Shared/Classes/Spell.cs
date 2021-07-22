using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

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
		protected double bonusCrit = 0;
		protected double coefficient = 0;
		protected decimal cooldownRemaining = decimal.Zero;
		protected string name = null;
		protected string varName = null; // Same as 'name' except it's written in camelCase
		protected string type = null;
		protected string school = null;
		protected string breakdownTable = "damage";
		protected bool isDot = false;
		protected bool doesDamage = false;
		protected bool canCrit = false;
		protected bool casting = false;
		protected bool isItem = false;
		protected bool onGcd = true;
		protected bool isProc = false;

		public Spell(Player p)
		{
			player = p;
		}

		public void Reset()
		{
			cooldownRemaining = decimal.Zero;
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

		public double CalculateCastTime()
		{
			return castTime;
		}
	}

	public class ShadowBolt : Spell
	{
		public ShadowBolt(Player p) : base(p)
		{
			castTime = CalculateCastTime();
			coefficient = 3 / 3.5; // check for Shadow and Flame
			minDmg = 544;
			maxDmg = 607;
			name = "Shadow Bolt";
			doesDamage = true;
			canCrit = true;
			school = SpellSchool.SHADOW;
			type = SpellType.DESTRUCTION;
			// call Setup() but check whether this constructor runs first or the base constructor. If base then just put Setup() in the base constructor
		}

		private double CalculateCastTime()
		{
			return 3; // check for Bane
		}
	}
}
