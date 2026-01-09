import styles from './index.module.sass';
import Effect from './Effect';
import { ColorPicker, NumberInput, Select } from '@mantine/core';
import { useState } from 'react';
import { QUICK_COLORS } from '../../storage/Colors';

const Effects = () => {
  const [flashlightColor, setFlashlightColor] = useState('#ffffff');

  return <>
    <div className="wrapper">
      <Effect label="Color shift" effect="colorshift">
        <div className={styles.inputs}>
          <Select
            label="Style"
            name="style"
            data={['global', 'perpixel']}
            defaultValue="global"
          />
          <NumberInput
            label="Change every ms"
            name="changeEveryMs"
            defaultValue="900"
          />
          <NumberInput
            label="Blend per second"
            name="blendPerSecond"
            defaultValue="120"
          />
        </div>
      </Effect>
      <Effect label="Fire" effect="fire">
        <div className={styles.inputs}>
          <NumberInput
            label="Cooling"
            name="cooling"
            defaultValue="55"
          />
          <NumberInput
            label="Sparking"
            name="sparking"
            defaultValue="120"
          />
          <NumberInput
            label="Step"
            name="stepMs"
            defaultValue="30"
          />
          <NumberInput
            label="Max Flares"
            name="maxFlares"
            defaultValue="3"
          />
          <NumberInput
            label="Flare Chance"
            name="flareChance"
            defaultValue="30"
          />
          <NumberInput
            label="Flare Rows"
            name="flareRows"
            defaultValue="7"
          />
          <NumberInput
            label="Flare Decay"
            name="flareDecay"
            defaultValue="14"
          />
        </div>
      </Effect>
      <Effect label="Flashlight" effect="flashlight">
        <div className={styles.inputs}>
          <ColorPicker
            format="hex"
            defaultValue="#ffffff"
            value={flashlightColor}
            onChange={setFlashlightColor}
            swatches={QUICK_COLORS}
          />
          <input type="hidden" name="color" value={flashlightColor} />
        </div>
      </Effect>
    </div>
  </>
}

export default Effects;