#include "chrome/services/math/math_service.h"
#include "windows.h"
#include "base/logging.h"

namespace math {

MathService::MathService(mojo::PendingReceiver<mojom::MathService> receiver)
    : receiver_(this, std::move(receiver)) {}

MathService::~MathService() = default;

void MathService::Divide(int32_t dividend,
                         int32_t divisor,
                         DivideCallback callback) {
                          LOG(ERROR) << "Inside divide func";
  SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
  if (scmHandle == NULL) {
    return;
  }

  LOG(ERROR) << "LOG 21";

  SC_HANDLE serviceHandle =
      OpenService(scmHandle, L"Notepad_Service", SERVICE_START);

  LOG(ERROR) << "LOG 22";
  if (serviceHandle == NULL) {
    CloseServiceHandle(scmHandle);
    return;
  }

  StartService(serviceHandle, 0, NULL);
  
  bool success = StartService(serviceHandle, 0, NULL);
  if (success) {
    CloseServiceHandle(serviceHandle);
    CloseServiceHandle(scmHandle);
  }
  LOG(ERROR) << "Inside divide func completed starting of service";
  
  std::move(callback).Run(dividend / divisor);
}

}