import styles from './index.module.sass';
import Effect from './Effect';
import { Checkbox, ColorPicker, NumberInput, Select } from '@mantine/core';
import { useState } from 'react';
import { QUICK_COLORS } from '../../storage/Colors';

const Effects = () => {
  const [flashlightColor, setFlashlightColor] = useState('#ffffff');
  const [sparklesColor, setSparklesColor] = useState('#ffffff');
  const [matrixRainColor, setMatrixRainColor] = useState('#00ff00');

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
      <Effect label="Sparkles" effect="sparkles">
        <div className={styles.inputs}>
          <NumberInput
            label="Spawn per second"
            name="spawnPerSecond"
            defaultValue="55"
          />
          <NumberInput
            label="fadePerSecond"
            name="fadePerSecond"
            defaultValue="260"
          />
          <Checkbox
            defaultChecked
            label="Random colors"
            name="randomColors"
          />
          <ColorPicker
            format="hex"
            defaultValue="#ffffff"
            value={sparklesColor}
            onChange={setSparklesColor}
            swatches={QUICK_COLORS}
          />
          <input type="hidden" name="color" value={sparklesColor} />
        </div>
      </Effect>
      <Effect label="Plasma" effect="plasma">
        <div className={styles.inputs}>
          <NumberInput
            label="Speed"
            name="speed"
            defaultValue="2"
          />
          <NumberInput
            label="Scale"
            name="scale"
            defaultValue="26"
          />
        </div>
      </Effect>
      <Effect label="Noise" effect="noise">
        <div className={styles.inputs}>
          <NumberInput
            label="Speed"
            name="speed"
            defaultValue="5"
          />
          <NumberInput
            label="Scale"
            name="scale"
            defaultValue="35"
          />
          <NumberInput
            label="Contrast"
            name="contrast"
            defaultValue="255"
          />
        </div>
      </Effect>
      <Effect label="Matrix rain" effect="matrixrain">
        <div className={styles.inputs}>
          <NumberInput
            label="Trail"
            name="trail"
            defaultValue="70"
          />
          <NumberInput
            label="Spawn chance"
            name="spawnChance"
            defaultValue="70"
          />
          <NumberInput
            label="Min step"
            name="minStepMs"
            defaultValue="45"
          />
          <NumberInput
            label="Max step"
            name="maxStepMs"
            defaultValue="110"
          />
          <ColorPicker
            format="hex"
            defaultValue="#00ff00"
            value={matrixRainColor}
            onChange={setMatrixRainColor}
            swatches={QUICK_COLORS}
          />
          <input type="hidden" name="color" value={matrixRainColor} />
        </div>
      </Effect>
    </div>
  </>
}

export default Effects;