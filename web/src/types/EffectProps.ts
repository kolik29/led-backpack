type EffectProps = {
  label: string;
  effect: 'flashlight' | 'colorshift' | 'fire' | 'sparkles' | 'plasma' | 'noise' | 'matrixrain' | 'snake' | 'ping';
  fields?: EffectPropsField[];
}

type EffectPropsField = {
  type: string;
  name: string;
  label: string;
  data?: any[];
  defaultValue?: any;
}

export default EffectProps;
export type { EffectPropsField };