import { Box, Button, Checkbox, Collapse, ColorPicker, Group, NumberInput, Select } from '@mantine/core'
import { useDisclosure } from '@mantine/hooks';
import styles from './index.module.sass';
import { setEffect } from '../../../Api/Requests';
import EffectProps, { EffectPropsField } from '../../../types/EffectProps';
import { QUICK_COLORS } from '../../../storage/Colors';
import { useMemo, useState } from 'react';

type Params = Record<string, any>;

const Effect = ({label = '', effect, fields = []}: EffectProps) => {
  const [opened, { toggle }] = useDisclosure(false);

  const initParams = useMemo(() => {
    const p: Params = {};
    for (const f of fields as any[]) {
      if (f.type === "checkbox") {
        p[f.name] = Boolean(f.defaultValue ?? false);
      } else if (f.type === "number") {
        p[f.name] = Number(f.defaultValue ?? 0);
      } else if (f.type === "color") {
        p[f.name] = String(f.defaultValue ?? "#ffffff");
      } else {
        p[f.name] = f.defaultValue ?? "";
      }
    }
    return p;
  }, [fields]);

  const [params, setParams] = useState(initParams);

  const setParam = (name: string, value: any) => {
    setParams((prev: any) => ({ ...prev, [name]: value }));
  };

  const buildPayloadParams = (): Params => {
    const out: Params = {};
    for (const f of fields as any[]) {
      const v = params[f.name];

      if (f.type === "number") out[f.name] = Number(v);
      else if (f.type === "checkbox") out[f.name] = Boolean(v);
      else out[f.name] = v;
    }
    return out;
  };

  const send = async (data: any) => {
    try {
      await setEffect({ effect, params: buildPayloadParams() });
    } catch (e) {
      console.error(e);
    }
  }

  return <>
    <Box className={styles.box}>
      <Group>
        <div onClick={toggle} className={styles.effect}>{label}</div>
      </Group>
  
      <Collapse in={opened}>
        <div className={styles.content}>
          {fields.map((field: EffectPropsField) => {
            if (field.type === 'color') {
              return<ColorPicker
                label={field.label}
                key={field.name}
                defaultValue={field.defaultValue}
                value={String(params[field.name] ?? '#ffffff')}
                onChange={(v) => setParam(field.name, v)}
                swatches={QUICK_COLORS}
                format="hex"
              />;
            } else if (field.type === 'number') {
              return <NumberInput
                label={field.label}
                key={field.name}
                defaultValue={field.defaultValue}
                value={params[field.name]}
                onChange={(v) => setParam(field.name, typeof v === "number" ? v : Number(v))}
              />;
            } else if (field.type === 'select') {
              return <Select
                label={field.label}
                key={field.name}
                defaultValue={field.defaultValue}
                value={params[field.name] ?? null}
                onChange={(v) => setParam(field.name, v ?? '')}
                data={field.data}
              />;
            } else if (field.type === 'checkbox') {
              return <Checkbox
                label={field.label}
                key={field.name}
                defaultValue={field.defaultValue}
                checked={Boolean(params[field.name])}
                onChange={(e) => setParam(field.name, e.currentTarget.checked)}
              />;
            }
          })}
          <Button
            w="100%"
            variant="filled"
            onClick={send}
          >Send</Button>
        </div>
      </Collapse>
    </Box>
  </>
}

export default Effect;