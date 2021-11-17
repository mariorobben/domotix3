using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Devices.Wago;
using DeviceHost.DevicesConfiguration;
using DeviceHost.Engine.MiLight;
using Domotix3.Devices.Wago;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;

namespace DeviceHost.Controllers
{
    [Route("devices/milight")]
    public class MiLightController : ControllerBase
    {
        public MiLightController(IDeviceRepository deviceRepository)
        {
            _deviceRepository = deviceRepository;
        }

        [HttpPut]
        [Route("allzonesoff/{name}")]
        public IActionResult AllZonesOff(string name)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.AllZonesOff();

            return Ok();
        }

        [HttpPut]
        [Route("allzoneson/{name}")]
        public IActionResult AllZonesOn(string name)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.AllZonesOn();

            return Ok();
        }

        [HttpPut]
        [Route("allzoneswhite/{name}")]
        public IActionResult AllZonesWhite(string name)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.AllZonesWhite();

            return Ok();
        }

        [HttpPut]
        [Route("zoneon/{name}")]
        public IActionResult ZoneOn(string name, [FromBody] Domotix3.Devices.MiLight.Zone zone)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.ZoneOn(zone);

            return Ok();
        }

        [HttpPut]
        [Route("zoneoff/{name}")]
        public IActionResult ZoneOff(string name, [FromBody] Domotix3.Devices.MiLight.Zone zone)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.ZoneOff(zone);

            return Ok();
        }

        [HttpPut]
        [Route("zonewhite/{name}")]
        public IActionResult ZoneWhite(string name, [FromBody] Domotix3.Devices.MiLight.Zone zone)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.ZoneWhite(zone);

            return Ok();
        }

        [HttpPut]
        [Route("brightness/{name}")]
        public IActionResult Brightness(string name, [FromBody] Domotix3.Devices.MiLight.Brightness brightness)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.Brightness(brightness);

            return Ok();
        }

        [HttpPut]
        [Route("hue/{name}")]
        public IActionResult Hue(string name, [FromBody] Domotix3.Devices.MiLight.Hue hue)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.Hue(hue);

            return Ok();
        }

        [HttpPut]
        [Route("hue/{name}")]
        public IActionResult DiscoIncSpeed(string name)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.DiscoIncSpeed();

            return Ok();
        }

        [HttpPut]
        [Route("hue/{name}")]
        public IActionResult DiscoDecSpeed(string name)
        {
            if (!_deviceRepository.TryGetDevice(name, out MiLightDevice miLightDevice))
            {
                return NotFound();
            }

            miLightDevice.DiscoDecSpeed();

            return Ok();
        }

        private readonly IDeviceRepository _deviceRepository;
    }
}
