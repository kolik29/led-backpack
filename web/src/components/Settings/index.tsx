import { Button, Slider, Text } from "@mantine/core";
import { clear, info, ping, reboot, setBrightness as sendBrightness } from "../../Api/Requests";
import { useState } from "react";
import styles from "./index.module.sass";

const Settings = () => {
  const [text, setText] = useState<any>();

  const onClear = async () => {
    try {
      await clear();
    } catch (e) {
      console.error(e);
    }
  }

  const onInfo = async () => {
    try {
      setText(await info());
    } catch (e) {
      console.error(e);
    }
  }

  const onPing = async () => {
    try {
      setText(await ping());
    } catch (e) {
      console.error(e);
    }
  }

  const onReboot = async () => {
    try {
      await reboot();
    } catch (e) {
      console.error(e);
    }
  }

  const onBrightness = async (value: number) => {
    try {
      await sendBrightness(value);
    } catch (e) {
      console.error(e);
    }
  }

  return <>
    <div className="wrapper">
      <Text size="sm" mt="xl">Brightness</Text>
      <div className={styles.brightness}>
        <Slider
          color="blue"
          defaultValue={32}
          onChangeEnd={onBrightness}
          max={64}
          marks={[
            { value: 0, label: '0%' },
            { value: 16, label: '25%' },
            { value: 32, label: '50%' },
            { value: 48, label: '75%' },
            { value: 64, label: '100%' },
          ]}
        />
      </div>
      <Button variant="filled" onClick={onClear}>Clear</Button>
      <Button variant="filled" onClick={onInfo}>Info</Button>
      <Button variant="filled" onClick={onPing}>Ping</Button>
      <Button variant="filled" onClick={onReboot}>Reboot</Button>
    </div>
    <pre>
      {JSON.stringify(text, null, 2)}
    </pre>
  </>;
};

export default Settings;