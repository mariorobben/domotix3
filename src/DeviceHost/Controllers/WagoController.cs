using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Devices.Wago;
using DeviceHost.DevicesConfiguration;
using Microsoft.AspNetCore.Mvc;

namespace DeviceHost.Controllers
{
    [Route("devices/wago")]
    public class WagoController : ControllerBase
    {
        public WagoController(IDeviceRepository deviceRepository)
        {
            _deviceRepository = deviceRepository;
        }

        [HttpGet]
        [Route("getterminals/{name}")]
        public IActionResult Get(string name)
        {
            if (!_deviceRepository.TryGetDevice(name, out WagoDevice wagoDevice))
            {
                return NotFound();
            }

            return Ok(wagoDevice.GetTerminals());
        }

        private readonly IDeviceRepository _deviceRepository;
    }
}
