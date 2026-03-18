# Windows Enumeration for Malware Dev

## Enumeration is the reconnaissance phase.
In malware development, it’s how a payload "looks around" to decide if it should execute, hide, or spread.

---

## 🔍 What is Enumeration?

It is the process of gathering information about the target Windows system. Malware uses this to find:

- **Security Software** – Is there an Antivirus or EDR running?
- **Environment** – Am I in a Virtual Machine or Sandbox? (Anti-VM)
- **Privileges** – Am I running as a Guest, User, or Admin?
- **Network** – What other computers can I see on this domain?
