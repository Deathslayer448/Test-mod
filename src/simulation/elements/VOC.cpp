#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_VOC()
{
	Identifier = "DEFAULT_PT_VOC";
	Name = "VOC";
	Colour = PIXPACK(0x222222);
	MenuVisible = 1;
	MenuSection = SC_GAS;
	Enabled = 1;

	Advection = 0.9f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.97f;
	Loss = 0.20f;
	Collision = 0.0f;
	Gravity = -0.2f;
	Diffusion = 0.0f;
	HotAir = 0.0006f * CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 1;

	HeatConduct = 140;
	Description = "Volatile Organic Compounds, Very Easy to burn.";

	Properties = TYPE_GAS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, rt, t = parts[i].type;
	for (rx = -2; rx < 3; rx++)
		for (ry = -2; ry < 3; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y + ry][x + rx];
				if (!r)
					continue;
				rt = TYP(r);

				int t = parts[i].temp - sim->pv[y / CELL][x / CELL];
				if (t >= 573.15 && RNG::Ref().chance(1, 2) && parts[i].ctype == PT_WOOD && rt == PT_FIRE) {
					parts[i].life = 150;
					sim->part_change_type(i, x, y, PT_FIRE);
					}
				else if (t >= 773.15 && RNG::Ref().chance(1, 100) && parts[i].ctype == PT_WOOD) {
					parts[i].life = 150;
					sim->part_change_type(i, x, y, PT_FIRE);
				}
				if (rt == PT_WOOD && parts[i].ctype == PT_NONE) {
					parts[i].ctype = PT_WOOD;
				}
				if (t >= 723.15 && RNG::Ref().chance(1, 2) && parts[i].ctype == PT_COAL && rt == PT_FIRE) {
					parts[i].life = 150;
					sim->part_change_type(i, x, y, PT_FIRE);
				}
				else if (t >= 823.15 && RNG::Ref().chance(1, 1000) && parts[i].ctype == PT_COAL) {
					parts[i].life = 150;
					parts[i].temp = parts[i].temp + RNG::Ref().between(1, 2);
					sim->part_change_type(i, x, y, PT_FIRE);
				}
				if (rt == PT_COAL && parts[i].ctype == PT_NONE)
					parts[i].ctype = PT_COAL;
			}

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	*colr = 55;
	*colg = 55;
	*colb = 55;

	*firea = 75;
	*firer = 55;
	*fireg = 55;
	*fireb = 55;

	*pixel_mode = PMODE_NONE; //Clear default, don't draw pixel
	*pixel_mode |= FIRE_BLEND;
	//Returning 1 means static, cache as we please
	return 1;
}

