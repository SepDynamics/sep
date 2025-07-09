export enum MemoryTierEnum {
  STM = 0,
  MTM = 1,
  LTM = 2,
  HOST = 100,
  DEVICE = 101,
  UNIFIED = 102
}

export interface MemoryTierConfig {
  type: MemoryTierEnum;
  size: number;
}
