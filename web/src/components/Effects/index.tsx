import EFFECTS from '../../storage/Effects';
import Effect from './Effect';
import { useState } from 'react';

const Effects = () => {
  return <>
    <div className="wrapper">
      {EFFECTS.map((effect: any, index: number) => <Effect key={index} label={effect.label} effect={effect.effect} fields={effect.fields} />)}
    </div>
  </>
}

export default Effects;