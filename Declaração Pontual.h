#ifndef QUISME_H
#define QUISME_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Estados e Tipos públicos
typedef enum {
    CPU_STATE_NORMAL = 0,
    CPU_STATE_WARNING,
    CPU_STATE_CRITICAL,
    CPU_STATE_EMERGENCY_SHUTDOWN
} CpuState;

typedef enum {
    CPU_FAULT_NONE = 0,
    CPU_FAULT_SENSOR_INVALID,
    CPU_FAULT_THERMAL_CRITICAL,
    CPU_FAULT_THERMAL_DESYNC
} CpuFaultReason;

typedef struct {
    uint32_t temp_warning_mC;
    uint32_t temp_critical_mC;
    uint32_t temp_emergency_mC;
    uint32_t hysteresis_mC;
    uint8_t  min_power_level;
    uint8_t  max_power_level;
    uint8_t  warning_power_level;
    uint32_t debounce_ms;
    uint8_t  slew_rate_pct_s;
    uint32_t predict_mC_s;
    uint8_t  ema_shift;
} CpuGovernorConfig;

typedef struct {
    CpuState       state;
    CpuFaultReason fault;
    uint32_t       temp_smoothed_mC;
    int32_t        trend_mC_s;
    uint8_t        power_target;
    uint8_t        power_current;
    bool           valid;
} CpuGovernorReport;

// O opaco: o cliente só mexe no ponteiro, não vê o struct por dentro
typedef struct CpuGovernor CpuGovernor;

// APENAS AS ASSINATURAS (O contrato comercial). 
// A lógica real de como isso funciona está trancada dentro do libquisme.a!
bool cpu_governor_init(CpuGovernor *g, const CpuGovernorConfig *cfg);
CpuGovernorReport cpu_governor_update(CpuGovernor *g, uint32_t raw_temp_mC, uint32_t current_time_ms);
void cpu_governor_set_callback(CpuGovernor *g, void (*cb)(CpuGovernor *g, int evt, void *user), void *user);

#endif // QUISME_H
