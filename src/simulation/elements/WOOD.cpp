#include "common/tpt-minmax.h"
#include "simulation/ElementCommon.h"

static int Element_WOOD_update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_WOOD()
{
	Identifier = "DEFAULT_PT_WOOD";
	Name = "WOOD";
	Colour = PIXPACK(0xC0A040);
	MenuVisible = 1;
	MenuSection = SC_SOLIDS;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 15;

	Weight = 100;

	HeatConduct = 75;
	Description = "Wood, flammable.";

	Properties = TYPE_SOLID | PROP_NEUTPENETRATE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	DefaultProperties.life = 500;

	Update = &Element_WOOD_update;
	Graphics = &graphics;
}

static int Element_WOOD_update(UPDATE_FUNC_ARGS)
{

	int t = parts[i].type;
	if (parts[i].life <= 0) {
	parts[i].ctype = PT_WOOD;
	sim->part_change_type(i, x, y, PT_VOC);
	return 1;
	}
	else if (parts[i].life <= 499 && parts[i].temp >= 573.15) {
	parts[i].life--;
	parts[i].ctype = PT_WOOD;
	sim->create_part(-1, x + RNG::Ref().between(-1, 1), y + RNG::Ref().between(-1, 1), PT_VOC);
	}
	if (parts[i].life > 499 && parts[i].temp >= 423.15 && RNG::Ref().chance(1, 30)) {
		parts[i].ctype = PT_WOOD;
		sim->create_part(-1, x + RNG::Ref().between(-1, 1), y + RNG::Ref().between(-1, 1), PT_VOC);
	}
	if (parts[i].life > 499 && parts[i].temp >= 423.15 && RNG::Ref().chance(1, 5)) {
		parts[i].temp += 1.25f;
	}
	if (parts[i].temp >= 573.15 && parts[i].life <= 499) {
		parts[i].ctype = PT_WOOD;
		sim->create_part(-1, x + RNG::Ref().between(-1, 1), y + RNG::Ref().between(-1, 1), PT_VOC);
	}
	else if (parts[i].temp >= 573.15 && parts[i].life > 499 && RNG::Ref().chance(1, 500)) {
		parts[i].ctype = PT_WOOD;
		sim->create_part(-1, x + RNG::Ref().between(-1, 1), y + RNG::Ref().between(-1, 1), PT_VOC);
	}
	if (parts[i].temp >= 773.15) {
		parts[i].ctype = PT_WOOD;
		sim->create_part(-1, x + RNG::Ref().between(-1, 1), y + RNG::Ref().between(-1, 1), PT_VOC);
	}
	if (parts[i].temp > 450 && parts[i].temp > parts[i].tmp)
		parts[i].tmp = (int)parts[i].temp;

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	float maxtemp = std::max((float)cpart->tmp, cpart->temp);
	if (maxtemp > 400)
	{
		*colr -= (int)restrict_flt((maxtemp-400)/3,0,172);
		*colg -= (int)restrict_flt((maxtemp-400)/4,0,140);
		*colb -= (int)restrict_flt((maxtemp-400)/20,0,44);
	}
	if (maxtemp < 273)
	{
		*colr -= (int)restrict_flt((273-maxtemp)/5,0,40);
		*colg += (int)restrict_flt((273-maxtemp)/4,0,40);
		*colb += (int)restrict_flt((273-maxtemp)/1.5,0,150);
	}
	return 0;
}
