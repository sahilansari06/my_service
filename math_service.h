#include "chrome/services/math/public/mojom/math_service.mojom.h"
#include "C:\Users\admin\Desktop\Chromium\src\mojo\public\cpp\bindings\receiver.h"
#include "C:\Users\admin\Desktop\Chromium\src\mojo\public\cpp\bindings\pending_receiver.h"
#include "windows.h"

namespace math {

class MathService : public mojom::MathService {
 public:
  explicit MathService(mojo::PendingReceiver<mojom::MathService> receiver);
  MathService(const MathService&) = delete;
  MathService& operator=(const MathService&) = delete;
  ~MathService() override;

 private:
  // mojom::MathService:
  void Divide(int32_t dividend,
              int32_t divisor,
              DivideCallback callback) override;

  mojo::Receiver<mojom::MathService> receiver_;
};

} 