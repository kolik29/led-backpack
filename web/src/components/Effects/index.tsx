import styles from './index.module.sass';
import Effect from './Effect';
import { ColorPicker, Select, TextInput } from '@mantine/core';
import { useState } from 'react';
import { QUICK_COLORS } from '../../storage/Colors';

const Effects = () => {
  const [flashlightColor, setFlashlightColor] = useState('#ffffff');

  return <>
    <div className='wrapper'>
      <Effect label="Color shift" effect="colorshift">
        <div className={styles.inputs}>
          <Select
            label="Style"
            name="style"
            data={['global', 'perpixel']}
            defaultValue="global"
          />
          <TextInput
            label="Change every ms"
            name="changeEveryMs"
            type="number"
            defaultValue="900"
          />
          <TextInput
            label="Blend per second"
            name="blendPerSecond"
            type="number"
            defaultValue="120"
          />
        </div>
      </Effect>
      <Effect label="Fire" effect="fire">
        <div className={styles.inputs}>
          <TextInput
            label="Cooling"
            name="cooling"
            type="number"
            defaultValue="55"
          />
          <TextInput
            label="Sparking"
            name="sparking"
            type="number"
            defaultValue="120"
          />
          <TextInput
            label="Step"
            name="stepMs"
            type="number"
            defaultValue="30"
          />
          <TextInput
            label="Max Flares"
            name="maxFlares"
            type="number"
            defaultValue="3"
          />
          <TextInput
            label="Flare Chance"
            name="flareChance"
            type="number"
            defaultValue="30"
          />
          <TextInput
            label="Flare Rows"
            name="flareRows"
            type="number"
            defaultValue="7"
          />
          <TextInput
            label="Flare Decay"
            name="flareDecay"
            type="number"
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