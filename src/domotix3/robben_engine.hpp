#ifndef _ROBBEN_ENGINE_H_
#define _ROBBEN_ENGINE_H_

#include <boost/shared_ptr.hpp>

// includes for the devices
#include <wago.hpp>

#include <engine.hpp>

#include <binary_input.hpp>
#include <binary_output.hpp>
#include <analog_output.hpp>
#include <shutters.hpp>

#include "manual_pir_light.hpp"
#include "sunset_sunrise.hpp"

#include <vector>

namespace domotix3
{

	class test_engine : public engine
	{
	public:
		test_engine(const std::shared_ptr< wago::wago_device >& wago_device);

	private:
		// wago stuff
		std::shared_ptr<wago::wago_device> wago_device_;
		wago::wago_io_bindings_engine wago_io_bindings_;

		binary_input binary_input_;
		binary_input binary_input2_;
		binary_output binary_output_;
		analog_output<uint16_t> dimmer_;
		dimmer_1_button_controller<uint16_t> controller_;
	};

	/*class slaapkamer_kind
	{
	public:
		slaapkamer_kind(
			engine& engine,
			binary_input& light_pushbutton,
			binary_input& shutter_up_pushbutton,
			binary_input& shutter_down_pushbutton,
			binary_output& light,
			shutter& shutter);

	private:
		binary_input& light_pushbutton_;
		binary_input& shutter_up_pushbutton_;
		binary_input& shutter_down_pushbutton_;
		binary_output& light_;
		shutter& shutter_;
		shutter_2_buttons_controller sc_;
	};

	class robben_engine : public engine
	{
	public:
		robben_engine(const std::shared_ptr< wago::wago_device >& wago_device);

	private:
		// wago stuff
		std::shared_ptr<wago::wago_device> wago_device_;
		wago::wago_io_bindings_engine wago_io_bindings_;
		
		std::array<binary_input, 3> inkomhal__trap__drukknoppen_; // 0, 1, 2
		std::array<binary_input, 8> leefruimte__inkomhal__drukknoppen_; // 3 - 10
		std::array<binary_input, 8> eetkamer__keuken__drukknoppen_; // 11 - 18
		std::array<binary_input, 2> eetkamer__muur_linkerkant__drukknoppen_; // 19 - 20
		std::array<binary_input, 4> eetkamer__terras__drukknoppen_; // 21 - 24
		std::array<binary_input, 3> keuken__tv__drukknoppen_; // 25 - 27
		binary_input keuken__werkblad_rechterkant__drukknop_; // 28
		std::array<binary_input, 3> keuken__werkblad_linkerkant__drukknoppen_; // 29 - 31
		binary_input rondom__achterdeur__pir_; // 32
		std::array<binary_input, 2> berging_beneden__garage__drukknoppen_; // 33 - 34
		std::array<binary_input, 2> garage__berging_beneden__drukknoppen_; // 35 - 36
		std::array<binary_input, 2> garage__achterdeur__drukknoppen_; // 37 - 38
		std::array<binary_input, 2> garage__garagepoort__drukknoppen_; // 39 - 40
		binary_input garage__garagepoort__magneetcontact_poort_dicht_; // 41
		binary_input garage__garagepoort__magneetcontact_poort_open_; // 42
		binary_input rondom__voordeur__pir_; // 43
		std::array<binary_input, 2> nachthal__trap_zolder__drukknoppen_; // 44 - 45
		binary_input toilet_boven__pir_; // 46 inverted
		std::array<binary_input, 3> slaapkamer_2__drukknoppen_; // 47 - 49
		std::array<binary_input, 3> slaapkamer_3__drukknoppen_; // 50 - 52
		std::array<binary_input, 3> slaapkamer_4__drukknoppen_; // 53 - 55
		std::array<binary_input, 3> slaapkamer_5__drukknoppen_; // 56 - 58
		std::array<binary_input, 3> badkamer__nachthal__drukknoppen_; // 59 - 61
		binary_input badkamer__meubel_rechts__drukknop_; // 62
		binary_input badkamer__meubel_links__drukknop_; // 63
		std::array<binary_input, 3> badkamer__slaapkamer_1__drukknoppen_; // 64 - 66
		std::array<binary_input, 5> slaapkamer_1__badkamer__drukknoppen_; // 67 - 71
		std::array<binary_input, 5> slaapkamer_1__nachthal__drukknoppen_; // 72 - 76
		std::array<binary_input, 2> nachthal__slaapkamer_1__drukknoppen_; // 77, 100
		std::array<binary_input, 10> slaapkamer_1__bed__drukknoppen_; // 78 - 87
		binary_input dressing__pir_; // 88
		binary_input nachthal__straat__pir_; // 89
		binary_input nachthal__tuin__pir_; // 90
		binary_input berging_boven__pir_; // 91
		std::array<binary_input, 2> zoldertrap__drukknoppen_; // 92 - 93
		std::array<binary_input, 4> bureau__drukknoppen_; // 94 - 97
		binary_input toilet_beneden__pir_; // 98
		binary_input tuin__tuinhuis__drukknop_; // 99

		binary_input nachthal_licht_drukknoppen_;
		binary_input nachthal_pirs_;

		binary_output leefruimte__spots_linksonder_; // 0
		binary_output leefruimte__spots_rechtsboven_; // 1
		binary_output leefruimte__spots_linksboven_rechtsonder_; // 2
		binary_output garage__licht_; // 3
		binary_output keuken__spoelbak__spots_; // 4
		binary_output slaapkamer_2__licht_; // 5
		binary_output berging_beneden__licht_; // 6
		binary_output eetkamer__tafel__licht_; // 7
		binary_output inkomhal__spots_; // 8
		binary_output leefruimte__salontafel__licht_; // 9
		binary_output keuken__eetplaats__licht_; // 10
		binary_output slaapkamer_3__licht_; // 11
		binary_output berging_boven__licht_; // 12
		//binary_output toilet_beneden__licht_; // 13
		binary_output eetkamer__spots_; // 14
		binary_output leefruimte__salontafel__vloer_stopcontact_; // 15
		binary_output leefruimte__salonzetel__vloer_stopcontact_; // 16
		binary_output leefruimte__bel_; // 17
		binary_output leefruimte__meubel__stopcontact_; // 18
		binary_output eetkamer__dressoir__stopcontact_; // 19
		binary_output slaapkamer_1__sterrenhemel__stopcontact_; // 20
		binary_output slaapkamer_1__tv__stopcontact_; // 21
		binary_output slaapkamer_1__wand_links__muurverlichting_; // 22
		binary_output slaapkamer_1__wand_rechts__muurverlichting_; // 23
		binary_output bureau__muur__vast_lichtpunt_1_; // 24
		binary_output bureau__muur__vast_lichtpunt_2_; // 25
		binary_output bureau__muur__vast_lichtpunt_3_; // 26
		binary_output keuken__werkblad__licht_; // 27
		binary_output badkamer__douche__licht_; // 28
		binary_output toilet_boven__licht_; // 29
		binary_output eetkamer__muur__vast_lichtpunt_1_; // 30
		binary_output eetkamer__muur__vast_lichtpunt_2_; // 31
		binary_output eetkamer__muur__vast_lichtpunt_3_; // 32
		binary_output dressing__spots_; // 33
		binary_output bureau__licht_; // 34
		binary_output keuken__keukenkasten__spots_; // 35
		binary_output zoldertrap__licht_; // 36
		binary_output slaapkamer_5__licht_; // 37
		binary_output zolder__licht_; // 38
		binary_output nachthal__bel_; // 39
		binary_output rondom__stopcontacten_; // 40
		binary_output tuin__stopcontacten_1_; // 41
		binary_output tuin__stopcontacten_2_; // 42
		binary_output rondom__halogeenspots_; // 43
		binary_output rondom__voordeur__spots_; // 44
		binary_output badkamer__hoeken__licht_; // 45
		binary_output slaapkamer_4__licht_; // 46
		binary_output tuin__straat__licht_; // 47
		binary_output tuin__terras__licht_; // 48
		binary_output rondom__garage__licht_; // 49
		binary_output rondom__terras__licht_; // 50
		binary_output badkamer__meubel__tl_; // 51
		binary_output badkamer__bad__spots_; // 52
		binary_output tuin__tuin__licht_; // 53
		binary_output tuin__tuinhuis__licht_; // 54
		//binary_output inkomhal__toilet_beneden__indicatie_; // 65
		binary_output inkomhal__leefruimte__indicatie_; // 66
		binary_output keuken__werkblad_links__indicatie_; // 67
		binary_output badkamer__nachthal__indicatie_; //  68
		binary_output badkamer__slaapkamer_1__indicatie_; // 69
		binary_output zoldertrap__indicatie_; // 70
		binary_output toilet_boven__led_verlichting_; // 71
		binary_output zolder__sturing_ventilatie_; // 72
		binary_output garage__sturing_motor_; // 73
		binary_output berging_beneden__parlofonie_; // 74

		binary_output toilet_beneden_licht_en_indicatie_; // 13 + 65

		analog_output<uint16_t> leefruimte__spots__dimmer_; // 0
		analog_output<uint16_t> slaapkamer_1__spots__dimmer_; // 1
		analog_output<uint16_t> nachthal__spots__dimmer_; // 2
		analog_output<uint16_t> slaapkamer_1__muurverlichting__dimmer_; // 3

		std::array<shutter, 5> rolluiken_;

		std::array<slaapkamer_kind, 4> slaapkamers_2_3_4_5_;

		std::vector<std::reference_wrapper<binary_output>> _alle_verlichting;

		manual_pir_light nachthal_;

		sunset_sunrise sunset_sunrise_;

	public:
		bool is_night() const;
	};*/

};

#endif
