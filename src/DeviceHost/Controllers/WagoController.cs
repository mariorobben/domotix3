using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using DeviceHost.Devices.Wago;
using DeviceHost.DevicesConfiguration;
using Domotix3.Devices.Wago;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;

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
        [Route("status/{name}")]
        public IActionResult GetStatus(string name)
        {
            if (!_deviceRepository.TryGetDevice(name, out WagoDevice wagoDevice))
            {
                return NotFound();
            }

            return Ok(wagoDevice.GetStatus());
        }

        [HttpGet]
        [Route("terminals/{name}")]
        public IActionResult GetTerminals(string name)
        {
            if (!_deviceRepository.TryGetDevice(name, out WagoDevice wagoDevice))
            {
                return NotFound();
            }

            return Ok(wagoDevice.GetTerminals());
        }

        [HttpGet]
        [Route("io/{name}")]
        public IActionResult Read(string name, [FromQuery] int offset, [FromQuery] int size)
        {
            if (!_deviceRepository.TryGetDevice(name, out WagoDevice wagoDevice))
            {
                return NotFound();
            }

            return Ok(wagoDevice.ReadRegion(new ReadRegion(offset, size)));
        }
        
        [HttpPut]
        [Route("io/{name}")]
        public IActionResult Write(string name, [FromQuery] int offset, [FromBody] byte[] bytes)
        {
            if (!_deviceRepository.TryGetDevice(name, out WagoDevice wagoDevice))
            {
                return NotFound();
            }

            wagoDevice.WriteRegion(new WriteRegion(offset, bytes));
            
            return Ok();
        }
        
        private readonly IDeviceRepository _deviceRepository;
    }
}
