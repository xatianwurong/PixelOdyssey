#pragma once
#include <string>

/**
 * @class Object
 * @brief 瀹炰綋瀵硅薄鍩虹被
 * @details 鎵€鏈夊彲娓叉煋瀹炰綋鐨勬娊璞″熀绫伙紝瀹氫箟浜嗗疄浣撶殑鍩烘湰鎺ュ彛
 * @coreResponsibility 鎻愪緵瀹炰綋鐨勫熀鏈鏋跺拰鍞竴鏍囪瘑
 * @designIntent 閲囩敤闈㈠悜瀵硅薄鐨勮璁℃€濇兂锛岄€氳繃缁ф壙瀹炵幇涓嶅悓绫诲瀷瀹炰綋鐨勭粺涓€绠＄悊
 * @useCase 浣滀负鍚勭鍙覆鏌撳璞★紙濡傜珛鏂逛綋銆佺悆浣撶瓑锛夌殑鍩虹被
 * @attention 鎵€鏈夊疄浣撶被蹇呴』瀹炵幇Draw鏂规硶
 */
class Object
{
public:
    Object();
    virtual ~Object() = default;
    virtual void Update(float deltaTime) {}
    virtual void Draw() = 0;

    std::string GetId() const { return m_id; }

protected:
    std::string m_id;
};

