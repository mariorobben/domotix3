#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define USE_REAL

#include "robben_engine.hpp"

using namespace std;

namespace domotix3
{

	test_engine::test_engine(const std::shared_ptr<wago::wago_device>& wago_device)
		: domotix3::engine()
		, wago_device_(wago_device)
		, wago_io_bindings_(*this, *wago_device_)
		, binary_input_{ *this, wago_io_bindings_.register_input<bool>(0) }
		, binary_input2_{ *this, wago_io_bindings_.register_input<bool>(1) }
		, binary_output_{*this, wago_io_bindings_.register_output<bool>(0)}
		, dimmer_{ *this, 1000, 30000, wago_io_bindings_.register_output<uint16_t>(0) }
		, controller_{binary_input2_, dimmer_, std::chrono::seconds(5), std::chrono::milliseconds(500) }
	{
		binary_input_.on_toggle().connect([this] { binary_output_.toggle_value(); });
	}

/*
	slaapkamer_kind::slaapkamer_kind(
		domotix3::engine& engine,
		domotix3::binary_input& light_pushbutton,
		domotix3::binary_input& shutter_up_pushbutton,
		domotix3::binary_input& shutter_down_pushbutton,
		domotix3::binary_output& light,
		domotix3::shutter& shutter)
		: light_pushbutton_(light_pushbutton)
		, shutter_up_pushbutton_(shutter_up_pushbutton)
		, shutter_down_pushbutton_(shutter_down_pushbutton)
		, light_(light)
		, shutter_(shutter)
		, sc_(shutter_, std::chrono::milliseconds(1000))
	{
		shutter_up_pushbutton_.on_set().connect([this] {sc_.start_move_up(); });
		shutter_up_pushbutton_.on_clear().connect([this] {sc_.stop_move_up(); });
		shutter_down_pushbutton_.on_set().connect([this] {sc_.start_move_down(); });
		shutter_down_pushbutton_.on_clear().connect([this] {sc_.stop_move_down(); });

		light_pushbutton_.on_toggle().connect([this] {light_.toggle_value(); });
		light_pushbutton_.on_interval().connect([this](size_t interval) { if (light_.get_value()) light_.play_path(output_paths{ { repeat_output_path(duty_cycle_output_path(0.5), 3), 3 }, {fixed_output_path(true), 7} }, false, ::std::chrono::seconds(10)); });
	}

	robben_engine::robben_engine(const shared_ptr< wago::wago_device >& wago_device)
		: domotix3::engine()
		, wago_device_(wago_device)
		, wago_io_bindings_(*this, *wago_device_)

		, inkomhal__trap__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(0), { std::chrono::milliseconds(1000) } },
			{ *this, wago_io_bindings_.register_input<bool>(1), { std::chrono::milliseconds(1000) } },
			{ *this, wago_io_bindings_.register_input<bool>(2), { std::chrono::milliseconds(1000) } }
		} }
		, leefruimte__inkomhal__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(3) },
			{ *this, wago_io_bindings_.register_input<bool>(4) },
			{ *this, wago_io_bindings_.register_input<bool>(5) },
			{ *this, wago_io_bindings_.register_input<bool>(6) },
			{ *this, wago_io_bindings_.register_input<bool>(7) },
			{ *this, wago_io_bindings_.register_input<bool>(8) },
			{ *this, wago_io_bindings_.register_input<bool>(9) },
			{ *this, wago_io_bindings_.register_input<bool>(10) }
		} }
		, eetkamer__keuken__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(11) },
			{ *this, wago_io_bindings_.register_input<bool>(12) },
			{ *this, wago_io_bindings_.register_input<bool>(13) },
			{ *this, wago_io_bindings_.register_input<bool>(14) },
			{ *this, wago_io_bindings_.register_input<bool>(15) },
			{ *this, wago_io_bindings_.register_input<bool>(16) },
			{ *this, wago_io_bindings_.register_input<bool>(17) },
			{ *this, wago_io_bindings_.register_input<bool>(18) }
		} }
		, eetkamer__muur_linkerkant__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(19) },
			{ *this, wago_io_bindings_.register_input<bool>(20) }
		} }
		, eetkamer__terras__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(21) },
			{ *this, wago_io_bindings_.register_input<bool>(22) },
			{ *this, wago_io_bindings_.register_input<bool>(23) },
			{ *this, wago_io_bindings_.register_input<bool>(24) }
		} }
		, keuken__tv__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(25) },
			{ *this, wago_io_bindings_.register_input<bool>(26) },
			{ *this, wago_io_bindings_.register_input<bool>(27) }
		} }
		, keuken__werkblad_rechterkant__drukknop_{ *this, wago_io_bindings_.register_input<bool>(28) }
		, keuken__werkblad_linkerkant__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(29) },
			{ *this, wago_io_bindings_.register_input<bool>(30) },
			{ *this, wago_io_bindings_.register_input<bool>(31) }
		} }
		, rondom__achterdeur__pir_{ *this, wago_io_bindings_.register_input<bool>(32) }
		, berging_beneden__garage__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(33) },
			{ *this, wago_io_bindings_.register_input<bool>(34) }
		} }
		, garage__berging_beneden__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(35) },
			{ *this, wago_io_bindings_.register_input<bool>(36) }
		} }
		, garage__achterdeur__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(37) },
			{ *this, wago_io_bindings_.register_input<bool>(38) }
		} }
		, garage__garagepoort__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(39) },
			{ *this, wago_io_bindings_.register_input<bool>(40) }
		} }
		, garage__garagepoort__magneetcontact_poort_dicht_{ *this, wago_io_bindings_.register_input<bool>(41) }
		, garage__garagepoort__magneetcontact_poort_open_{ *this, wago_io_bindings_.register_input<bool>(42) }
		, rondom__voordeur__pir_{ *this, wago_io_bindings_.register_input<bool>(43) }
		, nachthal__trap_zolder__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(44) },
			{ *this, wago_io_bindings_.register_input<bool>(45) }
		} }
		, toilet_boven__pir_{ *this, wago_io_bindings_.register_input<bool>(46) }
		, slaapkamer_2__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(47), { std::chrono::milliseconds(1000) } },
			{ *this, wago_io_bindings_.register_input<bool>(48) },
			{ *this, wago_io_bindings_.register_input<bool>(49) }
		} }
		, slaapkamer_3__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(50), { std::chrono::milliseconds(1000) } },
			{ *this, wago_io_bindings_.register_input<bool>(51) },
			{ *this, wago_io_bindings_.register_input<bool>(52) }
		} }
		, slaapkamer_4__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(53), { std::chrono::milliseconds(1000) } },
			{ *this, wago_io_bindings_.register_input<bool>(54) },
			{ *this, wago_io_bindings_.register_input<bool>(55) }
		} }
		, slaapkamer_5__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(56), { std::chrono::milliseconds(1000) } },
			{ *this, wago_io_bindings_.register_input<bool>(57) },
			{ *this, wago_io_bindings_.register_input<bool>(58) }
		} }
		, badkamer__nachthal__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(59) },
			{ *this, wago_io_bindings_.register_input<bool>(60) },
			{ *this, wago_io_bindings_.register_input<bool>(61) }
		} }
		, badkamer__meubel_rechts__drukknop_{ *this, wago_io_bindings_.register_input<bool>(62) }
		, badkamer__meubel_links__drukknop_{ *this, wago_io_bindings_.register_input<bool>(63) }
		, badkamer__slaapkamer_1__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(64) },
			{ *this, wago_io_bindings_.register_input<bool>(65) },
			{ *this, wago_io_bindings_.register_input<bool>(66) }
		} }
		, slaapkamer_1__badkamer__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(67) },
			{ *this, wago_io_bindings_.register_input<bool>(68) },
			{ *this, wago_io_bindings_.register_input<bool>(69) },
			{ *this, wago_io_bindings_.register_input<bool>(70) },
			{ *this, wago_io_bindings_.register_input<bool>(71) }
		} }
		, slaapkamer_1__nachthal__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(72) },
			{ *this, wago_io_bindings_.register_input<bool>(73) },
			{ *this, wago_io_bindings_.register_input<bool>(74) },
			{ *this, wago_io_bindings_.register_input<bool>(75) },
			{ *this, wago_io_bindings_.register_input<bool>(76) }
		} }
		, nachthal__slaapkamer_1__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(77) },
			{ *this, wago_io_bindings_.register_input<bool>(100), { std::chrono::milliseconds(1000) } }
		} }
		, slaapkamer_1__bed__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(78) },
			{ *this, wago_io_bindings_.register_input<bool>(79) },
			{ *this, wago_io_bindings_.register_input<bool>(80) },
			{ *this, wago_io_bindings_.register_input<bool>(81) },
			{ *this, wago_io_bindings_.register_input<bool>(82) },
			{ *this, wago_io_bindings_.register_input<bool>(83) },
			{ *this, wago_io_bindings_.register_input<bool>(84) },
			{ *this, wago_io_bindings_.register_input<bool>(85) },
			{ *this, wago_io_bindings_.register_input<bool>(86) },
			{ *this, wago_io_bindings_.register_input<bool>(87) }
		} }
		, dressing__pir_{ *this, wago_io_bindings_.register_input<bool>(88) }
		, nachthal__straat__pir_{ *this, wago_io_bindings_.register_input<bool>(89) }
		, nachthal__tuin__pir_{ *this, wago_io_bindings_.register_input<bool>(90) }
		, berging_boven__pir_{ *this, wago_io_bindings_.register_input<bool>(91) }
		, zoldertrap__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(92) },
			{ *this, wago_io_bindings_.register_input<bool>(93) }
		} }
		, bureau__drukknoppen_{ {
			{ *this, wago_io_bindings_.register_input<bool>(94) },
			{ *this, wago_io_bindings_.register_input<bool>(95) },
			{ *this, wago_io_bindings_.register_input<bool>(96) },
			{ *this, wago_io_bindings_.register_input<bool>(97) }
		} }
		, toilet_beneden__pir_{ *this, wago_io_bindings_.register_input<bool>(98) }
		, tuin__tuinhuis__drukknop_{ *this, wago_io_bindings_.register_input<bool>(99) }


		, nachthal_licht_drukknoppen_{ *this, exclusive_binary_getter({wago_io_bindings_.register_input<bool>(1) , wago_io_bindings_.register_input<bool>(44), wago_io_bindings_.register_input<bool>(100) }),{ std::chrono::milliseconds(1000) } }
		, nachthal_pirs_{ *this, exclusive_binary_getter({ wago_io_bindings_.register_input<bool>(89), wago_io_bindings_.register_input<bool>(90) }) }

		, leefruimte__spots_linksonder_{ *this, wago_io_bindings_.register_output<bool>(0) }
		, leefruimte__spots_rechtsboven_{ *this, wago_io_bindings_.register_output<bool>(1) }
		, leefruimte__spots_linksboven_rechtsonder_{ *this, wago_io_bindings_.register_output<bool>(2) }
		, garage__licht_{ *this, wago_io_bindings_.register_output<bool>(3) }
		, keuken__spoelbak__spots_{ *this, wago_io_bindings_.register_output<bool>(4) }
		, slaapkamer_2__licht_{ *this, wago_io_bindings_.register_output<bool>(5) }
		, berging_beneden__licht_{ *this, wago_io_bindings_.register_output<bool>(6) }
		, eetkamer__tafel__licht_{ *this, wago_io_bindings_.register_output<bool>(7) }
		, inkomhal__spots_{ *this, wago_io_bindings_.register_output<bool>(8) }
		, leefruimte__salontafel__licht_{ *this, wago_io_bindings_.register_output<bool>(9) }
		, keuken__eetplaats__licht_{ *this, wago_io_bindings_.register_output<bool>(10) }
		, slaapkamer_3__licht_{ *this, wago_io_bindings_.register_output<bool>(11) }
		, berging_boven__licht_{ *this, wago_io_bindings_.register_output<bool>(12) }
		//, toilet_beneden__licht_{ *this, wago_io_bindings_.register_output<bool>(13) }
		, eetkamer__spots_{ *this, wago_io_bindings_.register_output<bool>(14) }
		, leefruimte__salontafel__vloer_stopcontact_{ *this, wago_io_bindings_.register_output<bool>(15) }
		, leefruimte__salonzetel__vloer_stopcontact_{ *this, wago_io_bindings_.register_output<bool>(16) }
		, leefruimte__bel_{ *this, wago_io_bindings_.register_output<bool>(17) }
		, leefruimte__meubel__stopcontact_{ *this, wago_io_bindings_.register_output<bool>(18) }
		, eetkamer__dressoir__stopcontact_{ *this, wago_io_bindings_.register_output<bool>(19) }
		, slaapkamer_1__sterrenhemel__stopcontact_{ *this, wago_io_bindings_.register_output<bool>(20) }
		, slaapkamer_1__tv__stopcontact_{ *this, wago_io_bindings_.register_output<bool>(21) }
		, slaapkamer_1__wand_links__muurverlichting_{ *this, wago_io_bindings_.register_output<bool>(22) }
		, slaapkamer_1__wand_rechts__muurverlichting_{ *this, wago_io_bindings_.register_output<bool>(23) }
		, bureau__muur__vast_lichtpunt_1_{ *this, wago_io_bindings_.register_output<bool>(24) }
		, bureau__muur__vast_lichtpunt_2_{ *this, wago_io_bindings_.register_output<bool>(25) }
		, bureau__muur__vast_lichtpunt_3_{ *this, wago_io_bindings_.register_output<bool>(26) }
		, keuken__werkblad__licht_{ *this, wago_io_bindings_.register_output<bool>(27) }
		, badkamer__douche__licht_{ *this, wago_io_bindings_.register_output<bool>(28) }
		, toilet_boven__licht_{ *this, wago_io_bindings_.register_output<bool>(29) }
		, eetkamer__muur__vast_lichtpunt_1_{ *this, wago_io_bindings_.register_output<bool>(30) }
		, eetkamer__muur__vast_lichtpunt_2_{ *this, wago_io_bindings_.register_output<bool>(31) }
		, eetkamer__muur__vast_lichtpunt_3_{ *this, wago_io_bindings_.register_output<bool>(32) }
		, dressing__spots_{ *this, wago_io_bindings_.register_output<bool>(33) }
		, bureau__licht_{ *this, wago_io_bindings_.register_output<bool>(34) }
		, keuken__keukenkasten__spots_{ *this, wago_io_bindings_.register_output<bool>(35) }
		, zoldertrap__licht_{ *this, wago_io_bindings_.register_output<bool>(36) }
		, slaapkamer_5__licht_{ *this, wago_io_bindings_.register_output<bool>(37) }
		, zolder__licht_{ *this, wago_io_bindings_.register_output<bool>(38) }
		, nachthal__bel_{ *this, wago_io_bindings_.register_output<bool>(39) }
		, rondom__stopcontacten_{ *this, wago_io_bindings_.register_output<bool>(40) }
		, tuin__stopcontacten_1_{ *this, wago_io_bindings_.register_output<bool>(41) }
		, tuin__stopcontacten_2_{ *this, wago_io_bindings_.register_output<bool>(42) }
		, rondom__halogeenspots_{ *this, wago_io_bindings_.register_output<bool>(43) }
		, rondom__voordeur__spots_{ *this, wago_io_bindings_.register_output<bool>(44) }
		, badkamer__hoeken__licht_{ *this, wago_io_bindings_.register_output<bool>(45) }
		, slaapkamer_4__licht_{ *this, wago_io_bindings_.register_output<bool>(46) }
		, tuin__straat__licht_{ *this, wago_io_bindings_.register_output<bool>(47) }
		, tuin__terras__licht_{ *this, wago_io_bindings_.register_output<bool>(48) }
		, rondom__garage__licht_{ *this, wago_io_bindings_.register_output<bool>(49) }
		, rondom__terras__licht_{ *this, wago_io_bindings_.register_output<bool>(50) }
		, badkamer__meubel__tl_{ *this, wago_io_bindings_.register_output<bool>(51) }
		, badkamer__bad__spots_{ *this, wago_io_bindings_.register_output<bool>(52) }
		, tuin__tuin__licht_{ *this, wago_io_bindings_.register_output<bool>(53) }
		, tuin__tuinhuis__licht_{ *this, wago_io_bindings_.register_output<bool>(54) }
		//, inkomhal__toilet_beneden__indicatie_{ *this, wago_io_bindings_.register_output<bool>(65) }
		, toilet_beneden_licht_en_indicatie_{ *this, binary_setters({ wago_io_bindings_.register_output<bool>(13) , wago_io_bindings_.register_output<bool>(65) }) }
		, inkomhal__leefruimte__indicatie_{ *this, wago_io_bindings_.register_output<bool>(66) }
		, keuken__werkblad_links__indicatie_{ *this, wago_io_bindings_.register_output<bool>(67) }
		, badkamer__nachthal__indicatie_{ *this, wago_io_bindings_.register_output<bool>(68) }
		, badkamer__slaapkamer_1__indicatie_{ *this, wago_io_bindings_.register_output<bool>(69) }
		, zoldertrap__indicatie_{ *this, wago_io_bindings_.register_output<bool>(70) }
		, toilet_boven__led_verlichting_{ *this, wago_io_bindings_.register_output<bool>(71) }
		, zolder__sturing_ventilatie_{ *this, wago_io_bindings_.register_output<bool>(72) }
		, garage__sturing_motor_{ *this, wago_io_bindings_.register_output<bool>(73) }
		, berging_beneden__parlofonie_{ *this, wago_io_bindings_.register_output<bool>(74) }

		, leefruimte__spots__dimmer_{ *this, 1000, 30000, wago_io_bindings_.register_output<uint16_t>(0) }
		, slaapkamer_1__spots__dimmer_{ *this, 1000, 30000, wago_io_bindings_.register_output<uint16_t>(2) }
		, nachthal__spots__dimmer_{ *this, 1000, 30000, wago_io_bindings_.register_output<uint16_t>(4) }
		, slaapkamer_1__muurverlichting__dimmer_{ *this, 1000, 30000, wago_io_bindings_.register_output<uint16_t>(6) }

		, rolluiken_{ {
			{ *this, std::chrono::seconds(5), std::chrono::seconds(5), wago_io_bindings_.register_output<bool>(55), wago_io_bindings_.register_output<bool>(60) },
			{ *this, std::chrono::seconds(5), std::chrono::seconds(5), wago_io_bindings_.register_output<bool>(56), wago_io_bindings_.register_output<bool>(61) },
			{ *this, std::chrono::seconds(5), std::chrono::seconds(5), wago_io_bindings_.register_output<bool>(57), wago_io_bindings_.register_output<bool>(62) },
			{ *this, std::chrono::seconds(5), std::chrono::seconds(5), wago_io_bindings_.register_output<bool>(58), wago_io_bindings_.register_output<bool>(63) },
			{ *this, std::chrono::seconds(5), std::chrono::seconds(5), wago_io_bindings_.register_output<bool>(59), wago_io_bindings_.register_output<bool>(64) },
		} }

		, slaapkamers_2_3_4_5_{ {
			{ *this, slaapkamer_2__drukknoppen_[0], slaapkamer_2__drukknoppen_[1], slaapkamer_2__drukknoppen_[2], slaapkamer_2__licht_, rolluiken_[1] },
			{ *this, slaapkamer_3__drukknoppen_[0], slaapkamer_3__drukknoppen_[1], slaapkamer_3__drukknoppen_[2], slaapkamer_3__licht_, rolluiken_[2] },
			{ *this, slaapkamer_4__drukknoppen_[0], slaapkamer_4__drukknoppen_[1], slaapkamer_4__drukknoppen_[2], slaapkamer_4__licht_, rolluiken_[3] },
			{ *this, slaapkamer_5__drukknoppen_[0], slaapkamer_5__drukknoppen_[1], slaapkamer_5__drukknoppen_[2], slaapkamer_5__licht_, rolluiken_[4] },
		} }

		, _alle_verlichting { 
			inkomhal__spots_,
			leefruimte__spots_linksonder_, leefruimte__spots_rechtsboven_, leefruimte__spots_linksboven_rechtsonder_, leefruimte__salontafel__licht_,
			eetkamer__tafel__licht_, eetkamer__spots_, eetkamer__muur__vast_lichtpunt_1_, eetkamer__muur__vast_lichtpunt_2_, eetkamer__muur__vast_lichtpunt_3_,
			keuken__spoelbak__spots_, keuken__eetplaats__licht_, keuken__werkblad__licht_, keuken__keukenkasten__spots_,
			berging_beneden__licht_,
			garage__licht_,
			rondom__halogeenspots_,
			slaapkamer_1__sterrenhemel__stopcontact_, slaapkamer_1__wand_links__muurverlichting_, slaapkamer_1__wand_rechts__muurverlichting_, 
			slaapkamer_2__licht_,
			slaapkamer_3__licht_,
			slaapkamer_4__licht_,
			slaapkamer_5__licht_,
			badkamer__douche__licht_, badkamer__hoeken__licht_, badkamer__meubel__tl_, badkamer__bad__spots_,
			zoldertrap__licht_,
			bureau__licht_,
			zolder__licht_
		}

		, nachthal_{ *this, std::chrono::seconds(10), std::chrono::seconds(3), std::chrono::seconds(5) }

		,sunset_sunrise_()
	{
		inkomhal__trap__drukknoppen_[0].on_interval().connect([this](size_t interval)
		{
			for (auto bo : _alle_verlichting)
			{
				bo.get().set_value(false);
			}
		});
		inkomhal__trap__drukknoppen_[0].on_toggle().connect([this] { inkomhal__spots_.toggle_value(); });

		inkomhal__trap__drukknoppen_[2].on_toggle().connect([this]
		{
			rondom__voordeur__spots_.toggle_value();
		});
		inkomhal__trap__drukknoppen_[2].on_interval().connect([this](size_t interval)
		{ 
			for (auto& shutter : rolluiken_)
			{
				shutter.move_up();
			}
		});

		toilet_beneden__pir_.on_toggle().connect([this] {toilet_beneden_licht_en_indicatie_.play_path(fixed_output_path(true), false, ::std::chrono::seconds(5)); });

		// nachthal
		nachthal_licht_drukknoppen_.on_toggle().connect([this] { nachthal_.light_on_temporary_or_light_off(); });
		nachthal_licht_drukknoppen_.on_interval().connect([this](size_t interval) { nachthal_.light_on_forever(); });
		nachthal__straat__pir_.on_toggle().connect([this] { if (is_night()) nachthal_.pir_triggered(); });
		nachthal__tuin__pir_.on_toggle().connect([this] { if (is_night())nachthal_.pir_triggered(); });
		nachthal_.on_light().connect([this](manual_pir_light::light_states light_state)
		{
			switch (light_state)
			{
			case manual_pir_light::light_states::on_manual:
				nachthal__spots__dimmer_.set_value(nachthal__spots__dimmer_.get_max_value());
				break;
			case manual_pir_light::light_states::on_pir:
				nachthal__spots__dimmer_.set_value(nachthal__spots__dimmer_.get_max_value() / 4);
				break;
			case manual_pir_light::light_states::off:
				nachthal__spots__dimmer_.set_value(0);
				break;
			}
		});

		// toilet boven
		toilet_boven__pir_.on_toggle().connect([this]
		{
			binary_output* select_output = nullptr;
			if (toilet_boven__licht_.get_value()) select_output = &toilet_boven__licht_;
			else if (toilet_boven__led_verlichting_.get_value()) select_output = &toilet_boven__led_verlichting_;
			else
			{
				select_output = (is_night() && nachthal_.get_state() == manual_pir_light::states::pir_temp_on) ? &toilet_boven__led_verlichting_ : &toilet_boven__licht_;
			}
			select_output->play_path(fixed_output_path(true), false, ::std::chrono::seconds(5));
		});

		// licht berging buiten
		rondom__achterdeur__pir_.on_toggle().connect([this] {if (!sunset_sunrise_.is_sun_up()) rondom__garage__licht_.play_path(fixed_output_path(true), false, ::std::chrono::seconds(5)); });
	}

	bool robben_engine::is_night() const
	{
		return engine::context()->local_time().time_of_day().hours() >= 22 || engine::context()->local_time().time_of_day().hours() < 6;
	}
	*/
};
	  