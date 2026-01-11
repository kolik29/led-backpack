type EffectProps = {
  label: string;
  effect: 'flashlight' | 'colorshift' | 'fire' | 'sparkles' | 'plasma' | 'noise' | 'matrixrain';
  children?: React.ReactNode;
}

export default EffectProps